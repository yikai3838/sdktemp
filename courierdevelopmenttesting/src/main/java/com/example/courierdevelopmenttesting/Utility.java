package com.example.courierdevelopmenttesting;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.support.v4.app.ActivityCompat;

public class Utility {

    private Activity activity;
    private ActivationCallback activationCallback;

    public Utility(Activity _activity) {
        activity = _activity;
    }

    static boolean isConnectedToNetwork(Activity activity) {
        ConnectivityManager conMgr = (ConnectivityManager)activity.getSystemService(Context.CONNECTIVITY_SERVICE);
        return (conMgr.getActiveNetworkInfo() != null);
    }

    public boolean checkPermission() {
        return (ActivityCompat.checkSelfPermission(activity, Manifest.permission.INTERNET) == PackageManager.PERMISSION_GRANTED);
    }

    public boolean isActivated() {
        return native_isActivated();
    }

    public void activation(final String userId, ActivationCallback _callback) {

        if (userId.isEmpty()) {
            activationCallback.onError(ActivationError.MISSING_PARAMETERS);
            return;
        }

        if (!checkPermission()) {
            activationCallback.onError(ActivationError.PERMISSION_DENIED);
            return;
        }

        activationCallback = _callback;

        if (!isConnectedToNetwork(activity)) {
            activationCallback.onError(ActivationError.NETWORK_ERROR);
            return;
        }

        boolean res = native_activate(userId);

        if (res) {
            activationCallback.onSuccess();
        } else {
            activationCallback.onError(ActivationError.NETWORK_ERROR);
        }

    }

    // Import Native Library
    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("native-lib");
    }

    private native boolean native_activate(String userId);
    private native boolean native_isActivated();
}
