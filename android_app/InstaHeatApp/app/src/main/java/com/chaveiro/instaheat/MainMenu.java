package com.chaveiro.instaheat;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import android.os.Handler;

import com.chaveiro.instaheat.domain.Utils;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class MainMenu extends AppCompatActivity {

    Handler tempHandler = new Handler(){
        @Override
        public void handleMessage(Message msg){
            currentTemp = findViewById(R.id.current_temp);
            currentTemp.setText((String) msg.obj);
        }
    };

    private Spinner tempsSpinner;
    private Spinner tapsSpinner;
    private Button startButton;
    private Button stopButton;
    private Button settingsButton;
    private Button statsButton;
    private TextView currentTemp;

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    public static final int MAX_TEMP = 75;

    private String opModeText = "Auto";
    private String regionText = "Porto";
    private String unitsText = "Celsius";

    private TextView opModeView;
    private TextView regionView;
    private TextView unitsView;

    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice device;
    private BluetoothSocket socket;

    private final static int REQUEST_ENABLE_BT = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //conf spinners
        setTapsSpinner();
        setTempsSpinner();

        //conf buttons
        confStartButton();
        confStopButton();

        //conf activities starters
        confSettingsButton();
        confStatsButton();
        confSettingsButton();


        //bluetooth conf
        confBluetoothAdapter();


        //current temp display
        confCurrentTemp();

        //conf status
        confStatus();

    }

    private void confStatus() {
        opModeView = findViewById(R.id.op_mode_text);
        regionView = findViewById(R.id.region_text);
        unitsView = findViewById(R.id.units_text);
        confStatusViewText();
    }

    private void confStatusViewText() {
        opModeView.setText(opModeText);
        regionView.setText(regionText);
        unitsView.setText(unitsText);
    }

    private void confBluetoothAdapter() {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            Toast.makeText(getApplicationContext(), "Bluetooth Not Supported", Toast.LENGTH_SHORT).show();
        } else {
            if (!mBluetoothAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
            device = mBluetoothAdapter.getBondedDevices().iterator().next();
            Toast.makeText(getApplicationContext(), device.getName() + "\n" + device.getAddress(), Toast.LENGTH_SHORT).show();
        }
    }

    private void confStartButton() {
        startButton = findViewById(R.id.start_button);
        startButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                    socket.connect();
                    socket.getOutputStream().write("1".getBytes());
                    Toast.makeText(getApplicationContext(), "Connection Successful", Toast.LENGTH_SHORT).show();
                    socket.close();
                } catch (Exception e) {
                    Toast.makeText(getApplicationContext(), "Connection Setup Failed\n" + e.getMessage(), Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void confStopButton() {
        stopButton = findViewById(R.id.stop_button);
        stopButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    socket = device.createRfcommSocketToServiceRecord(myUUID);
                    socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                    socket.connect();
                    socket.getOutputStream().write("0".getBytes());
                    Toast.makeText(getApplicationContext(), "Connection Successful", Toast.LENGTH_SHORT).show();
                    socket.close();
                } catch (Exception e) {
                    Toast.makeText(getApplicationContext(), "Connection Setup Failed\n" + e.getMessage(), Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void confCurrentTemp() {
        Runnable r = new Runnable() {
            @Override
            public void run() {
                while (true) {
                    try {
                        Thread.sleep(2000);
                        socket = device.createRfcommSocketToServiceRecord(myUUID);
                        socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                        socket.connect();
                        byte[] tempRead = new byte[256]; // for analog input
                        socket.getInputStream().read(tempRead);
                        String sRead = new String(tempRead);
                        String[] sSplit = sRead.split("\n");
                        Message msg = Message.obtain();
                        msg.obj = sSplit[0];
                        msg.setTarget(tempHandler);
                        msg.sendToTarget();
                        socket.close();
                    } catch (Exception e) {
                        //do nothing, can't show toast on worker threads
                    }
                }
            }
        };
        Thread updateTemps = new Thread(r);
        updateTemps.start();
    }

    private void confSettingsButton() {
        settingsButton = findViewById(R.id.settings_button);
        settingsButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(MainMenu.this, Settings.class);
                startActivityForResult(intent, 1);
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        if (requestCode == 1) {
            if (resultCode == MainMenu.RESULT_OK) {
                unitsText = data.getStringExtra("units");
                regionText = data.getStringExtra("region");
                confStatusViewText();
                setTempsSpinner();
            }
        }
    }

    private void confStatsButton() {
        statsButton = findViewById(R.id.stats_button);
        statsButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(MainMenu.this, Statistics.class);
                startActivity(intent);
            }
        });
    }

    private void setTempsSpinner() {
        List<String> temps = new ArrayList<>();
        setTempsWithUnits(temps);
        tempsSpinner = findViewById(R.id.temp_spinner);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_spinner_item, temps);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        tempsSpinner.setAdapter(adapter);
        tempsSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {
                if (getTemperature() != 0) {
                    Toast.makeText(getApplicationContext(), getTemperature() + "ºC", Toast.LENGTH_SHORT).show();
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parentView) {
                // your code here
            }
        });
    }

    private void setTempsWithUnits(List<String> temps) {
        switch (unitsText) {
            case ("Celsius"): {
                for (int i = 0; i < MAX_TEMP; i++) {
                    temps.add("" + i + "ºC");
                }
                break;
            }
            case ("Fahrenheit"): {
                for (int i = 0; i < MAX_TEMP; i++) {
                    temps.add("" + String.format("%.1f", Utils.CelsiusToFahrenheit(i)) + "ºF");
                }
                break;
            }
            case ("Kelvin"): {
                for (int i = 0; i < MAX_TEMP; i++) {
                    temps.add("" + Utils.CelsiusToKelvin(i) + "K");
                }
                break;
            }
        }
    }

    private void setTapsSpinner() {
        List<String> taps = new ArrayList<>();
        taps.add("Tap 1");
        tapsSpinner = findViewById(R.id.tap_spinner);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_spinner_item, taps);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        tapsSpinner.setAdapter(adapter);
    }

    private int getTemperature() {
        return tempsSpinner.getSelectedItemPosition();
    }
}
