package com.chaveiro.instaheat;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

public class Settings extends AppCompatActivity {

    private Button okButton;
    private Spinner regionsSpinner;
    private Spinner unitsSpinner;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        confSpinners();

        confOkButton();
    }

    private void confSpinners() {
        regionsSpinner = findViewById(R.id.region_spinner);
        unitsSpinner = findViewById(R.id.units_spinner);
    }

    private void confOkButton() {
        okButton = findViewById(R.id.ok_button );
        okButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent returnIntent = new Intent();
                returnIntent.putExtra("units", unitsSpinner.getSelectedItem().toString());
                returnIntent.putExtra("region", regionsSpinner.getSelectedItem().toString());
                setResult(MainMenu.RESULT_OK, returnIntent);
                Settings.this.finish();
            }
        });
    }
}
