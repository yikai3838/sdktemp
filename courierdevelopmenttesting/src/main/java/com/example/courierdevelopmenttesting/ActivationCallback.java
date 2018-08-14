package com.example.courierdevelopmenttesting;

public interface ActivationCallback {
    void onSuccess();
    void onError(ActivationError err);
}
