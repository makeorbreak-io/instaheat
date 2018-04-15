package com.chaveiro.instaheat;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ContentValues;
import android.content.Intent;
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

//import com.chaveiro.instaheat.domain.AppData;
import com.chaveiro.instaheat.domain.Utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;

public class MainMenu extends AppCompatActivity {

    private Spinner tempsSpinner;
    private Spinner tapsSpinner;
    private Button settingsButton;
    private Button statsButton;
    private TextView currentTemp;
    private Button ledOnButton;
    private Button ledOffButton;

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
    //private AppData database;
    private double time = 0;

    private final static int REQUEST_ENABLE_BT = 1;

    Handler tempHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            currentTemp = findViewById(R.id.current_temp);
            switch (unitsText) {
                case ("Celsius"): {
                    currentTemp.setText((msg.obj) + "ºC");
                    break;
                }
                case ("Fahrenheit"): {
                    double fTemp = Utils.CelsiusToFahrenheit(Double.parseDouble((String) msg.obj));
                    currentTemp.setText(fTemp + "ºF");
                    break;
                }
                case ("Kelvin"): {
                    double kTemp = Utils.CelsiusToKelvin(Double.parseDouble((String) msg.obj));
                    currentTemp.setText(kTemp + "K");
                    break;
                }
            }
            //ContentValues values = new ContentValues();
            //values.put("temperature", (String )msg.obj);
            //values.put("time", time);
            //time+=1.5;
            //database.getWritableDatabase().insert(AppData.TABLE_NAME, null,  values);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //conf spinners
        setTapsSpinner();
        setTempsSpinner();

        //init database
        //confDb();

        //conf activities starters
        confSettingsButton();
        confStatsButton();
        confSettingsButton();


        //bluetooth conf
        confBluetoothAdapter();

        //conf buttons
        confLedOnButton();
        confLedOffButton();

        //current temp display
        confCurrentTemp();

        //conf status
        confStatus();

    }

    private void confDb() {
        //database = new AppData(getApplicationContext());
    }

    @Override
    public void onBackPressed() {
        try{
            socket.close();
        } catch(Exception e){
            //
        }
        finish();
        return;
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
            while(!mBluetoothAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
            Intent intent = new Intent();
            BluetoothDevice device = intent.getParcelableExtra("android.bluetooth.device.extra.DEVICE");
            try{
                device.getClass().getMethod("setPairingConfirmation", boolean.class).invoke(device, true);
                device.getClass().getMethod("cancelPairingUserInput").invoke(device);
            } catch(Exception e){
                //Toast.makeText(getApplicationContext(), "Couldn't start pairing process" , Toast.LENGTH_SHORT).show();
            }
            device = mBluetoothAdapter.getBondedDevices().iterator().next();
            Toast.makeText(getApplicationContext(), device.getName() + "\n" + device.getAddress(), Toast.LENGTH_SHORT).show();
            boolean thrown = true;
            while(thrown){
                try{
                    socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                    socket.connect();
                    thrown = false;
                }catch(Exception e){
                    Toast.makeText(getApplicationContext(), "Connection Setup Failed\n" + e.getMessage(), Toast.LENGTH_SHORT).show();
                }
            }
        }
    }

    private void confLedOnButton() {
        ledOnButton = findViewById(R.id.led_on);
        ledOnButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    //socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                    //socket.connect();
                    socket.getOutputStream().write(("1").getBytes());
                    Toast.makeText(getApplicationContext(), "Connection Successful", Toast.LENGTH_SHORT).show();
                    //socket.close();
                } catch (Exception e) {
                    Toast.makeText(getApplicationContext(), "Connection Setup Failed\n" + e.getMessage(), Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void confLedOffButton() {
        ledOffButton = findViewById(R.id.led_off);
        ledOffButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    //socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                    //socket.connect();
                    socket.getOutputStream().write(("0").getBytes());
                    Toast.makeText(getApplicationContext(), "Connection Successful", Toast.LENGTH_SHORT).show();
                    //socket.close();
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
                        Thread.sleep(1500);
                        //socket = device.createRfcommSocketToServiceRecord(myUUID);
                        //socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                        //socket.connect();
                        byte[] tempRead = new byte[256]; // for analog input
                        socket.getInputStream().read(tempRead);
                        String sRead = new String(tempRead);
                        String[] sSplit = sRead.split("\n");
                        Message msg = Message.obtain();
                        msg.obj = sSplit[0];
                        msg.setTarget(tempHandler);
                        msg.sendToTarget();
                        //socket.close();
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
                    try {
                        //socket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
                        //socket.connect();
                        int tTemperature = tempsSpinner.getSelectedItemPosition();
                        switch(tTemperature){
                            case(25):{
                                tTemperature = 2;
                                break;
                            }
                            case(27):{
                                tTemperature = 3;
                                break;
                            }
                            case(29):{
                                tTemperature = 4;
                                break;
                            }
                            case(31):{
                                tTemperature = 5;
                                break;
                            }
                            case(33):{
                                tTemperature = 6;
                                break;
                            }
                            case(36):{
                                tTemperature = 7;
                                break;
                            }
                            case(38):{
                                tTemperature = 8;
                                break;
                            }
                            case(39):{
                                tTemperature = 9;
                                break;
                            }
                        }
                        socket.getOutputStream().write((""+tTemperature).getBytes());
                        //socket.close();
                        Toast.makeText(getApplicationContext(), getTemperature() + "ºC", Toast.LENGTH_SHORT).show();
                    } catch (Exception e) {
                        Toast.makeText(getApplicationContext(), "Could not Set Target Temperature", Toast.LENGTH_SHORT).show();
                    }
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
                    temps.add("" + Utils.CelsiusToFahrenheit(i) + "ºF");
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
