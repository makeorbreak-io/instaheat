package com.chaveiro.instaheat;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainMenu extends AppCompatActivity {

    private Spinner tempsSpinner;
    private Button startButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setTempsSpinner();
        confButton();

        getSelectedTap();
    }

    private void confButton() {
        startButton = findViewById(R.id.startButton);
        startButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "TEMP: " + tempsSpinner.getSelectedItem().toString(), Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void setTempsSpinner() {
        List<String> temps = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            temps.add(""+i);
        }
        tempsSpinner = findViewById(R.id.temp_spinner);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, temps);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        tempsSpinner.setAdapter(adapter);
    }

    private void getSelectedTap(){
        Spinner spinner = findViewById(R.id.temp_spinner);
        String abc = spinner.getSelectedItem().toString();
        Toast.makeText(getApplicationContext(), abc, Toast.LENGTH_SHORT).show();
    }
}
