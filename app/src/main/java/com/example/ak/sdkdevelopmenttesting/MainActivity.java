package com.example.ak.sdkdevelopmenttesting;

import android.graphics.Bitmap;
import android.net.http.SslError;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.SslErrorHandler;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;

import com.example.courierdevelopmenttesting.ActivationCallback;
import com.example.courierdevelopmenttesting.ActivationError;
import com.example.courierdevelopmenttesting.HelloWorld;
import com.example.courierdevelopmenttesting.Utility;

public class MainActivity extends AppCompatActivity {


    Utility utility;

    Button isActivatedButton, activationButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_testing);

        utility = new Utility(this);

        isActivatedButton = findViewById(R.id.isActivatedButton);
        isActivatedButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.e("isActivatedButton", utility.isActivated() + "");
            }
        });

        activationButton = findViewById(R.id.activationButton);
        activationButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                utility.activation("sdkdevtesting", new ActivationCallback() {
                    @Override
                    public void onSuccess() {
                        Log.e("activationButton", "successful");
                    }

                    @Override
                    public void onError(ActivationError err) {

                        Log.e("activationButton", "error: " + err);
                    }
                });
            }
        });

    }

    @Override
    protected void onResume() {
        super.onResume();
    }








}
