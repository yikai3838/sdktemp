package com.example.courierdevelopmenttesting;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.support.v4.app.ActivityCompat;

public class Activation {

    private Activity activity;
    private ActivateCallback activationCallback;

    public Activation(Activity _activity) {
        activity = _activity;
    }

    static boolean isConnectedToNetwork(Activity activity) {
        ConnectivityManager conMgr = (ConnectivityManager)activity.getSystemService(Context.CONNECTIVITY_SERVICE);
        return (conMgr.getActiveNetworkInfo() != null);
    }

    private boolean checkPermission() {
        return (ActivityCompat.checkSelfPermission(activity, Manifest.permission.INTERNET) == PackageManager.PERMISSION_GRANTED);
    }

    public boolean isActivated() {
        return nIsActivated();
    }

    public void activate(final String userId, ActivateCallback _callback) {

        if (userId.isEmpty()) {
            activationCallback.onError(ActivateError.MISSING_PARAMETERS);
            return;
        }

        if (!checkPermission()) {
            activationCallback.onError(ActivateError.PERMISSION_DENIED);
            return;
        }

        activationCallback = _callback;

        if (!isConnectedToNetwork(activity)) {
            activationCallback.onError(ActivateError.NETWORK_ERROR);
            return;
        }

        boolean res = nActivate(userId);

        if (res) {
            activationCallback.onSuccess();
        } else {
            activationCallback.onError(ActivateError.NETWORK_ERROR);
        }

    }

    // Import Native Library
    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("native-lib");
    }

    private native boolean nActivate(String userId);
    private native boolean nIsActivated();
}
