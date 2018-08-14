package com.example.ak.sdkdevelopmenttesting;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.courierdevelopmenttesting.ActivateCallback;
import com.example.courierdevelopmenttesting.Activation;
import com.example.courierdevelopmenttesting.ActivateError;

public class MainActivity extends AppCompatActivity {


    Activation sdkActivation;

    Button isActivatedButton, activationButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_testing);

        sdkActivation = new Activation(this);

        isActivatedButton = findViewById(R.id.isActivatedButton);
        isActivatedButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.e("isActivatedButton", sdkActivation.isActivated() + "");
            }
        });

        activationButton = findViewById(R.id.activationButton);
        activationButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                sdkActivation.activate("sdkdevtesting", new ActivateCallback() {
//                    @Override
//                    public void onSuccess() {
//                        Log.e("activationButton", "successful");
//                    }
//
//                    @Override
//                    public void onError(ActivateError err) {
//
//                        Log.e("activationButton", "error: " + err);
//                    }
//                });
            }
        });

    }

    @Override
    protected void onResume() {
        super.onResume();
    }








}
