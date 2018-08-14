package com.example.courierdevelopmenttesting;

public interface GetPaymentRecordCallback {
    void onSuccess();
    void onError(GetPaymentRecordError err);
}
