package com.example.ak.sdkdevelopmenttesting;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.courierdevelopmenttesting.ActivationCallback;
import com.example.courierdevelopmenttesting.ActivationError;
import com.example.courierdevelopmenttesting.Utility;

public class Testing extends AppCompatActivity {


//
//    private HelloWorld helloWorld;
//    private WebView theWebView;
//    private Button startButton, stopButton, refreshButton;
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
//
//        helloWorld = new HelloWorld(this);
//
//        theWebView = findViewById(R.id.theWebView);
//        theWebView.setWebViewClient(new WebViewClient() {
//
//            // 复写shouldOverrideUrlLoading()方法，使得打开网页时不调用系统浏览器， 而是在本WebView中显示
//            @Override
//            public boolean shouldOverrideUrlLoading(WebView view, String url) {
//                view.loadUrl(url);
//
//                Log.e("shouldOverrideUrlLoadi", "called");
//
//                return true;
//                //开始载入页面调用的，我们可以设定一个loading的页面，告诉用户程序在等待网络响应。
//            }
//
//
//            // 当每一个页面加载出来时的动作 可以获取当前页面的信息 如URL 如标题等
//            @Override
//            public void  onPageStarted(WebView view, String url, Bitmap favicon) {
//                //设定加载开始的操作
//                // 如可以得到当前的URL
//                //current_url=view.getUrl();
//                Log.e("onPageStarted", "called");
//            }
//
//
//            @Override
//            public void onPageFinished(WebView view, String url) {
//                //设定加载结束的操作
//                Log.e("onPageFinished", "called");
//            }
//
//            @Override
//            public void onLoadResource(WebView view, String url) {
//                //设定加载资源的操作,在加载页面资源时会调用，每一个资源（比如图片）的加载都会调用一次。
//                Log.e("onLoadResource", "called");
//            }
//
//            //加载页面的服务器出现错误时（如404）调用 使用自定义的错误界面 更符合软件的整体设计风格
//            //步骤1：写一个html文件（error_handle.html），用于出错时展示给用户看的提示页面
//            //步骤2：将该html文件放置到代码根目录的assets文件夹下
//            //步骤3：复写WebViewClient的onRecievedError方法
//            //该方法传回了错误码，根据错误类型可以进行不同的错误分类处理
//            @Override
//            public void onReceivedError(WebView view, int errorCode, String description, String failingUrl){
//                Log.e("onReceivedError", "called");
////
////                switch(errorCode)
////                {
////                    case HttpStatus.SC_NOT_FOUND:
////                        view.loadUrl("file:///android_assets/error_handle.html");
////                        break;
////                }
//            }
//
//            //处理https请求
//            @Override
//            public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
//
//                handler.proceed();    //表示等待证书响应
//                // handler.cancel();      //表示挂起连接，为默认方式
//                // handler.handleMessage(null);    //可做其他处理
//
//                Log.e("onReceivedSslError", "called");
//            }
//
//        });
//
//
//        startButton = findViewById(R.id.startButton);
//        startButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Log.e("startButton", "clicked");
//                theWebView.loadUrl("https://1mme.com/qr");
//            }
//        });
//
//        stopButton = findViewById(R.id.stopButton);
//        stopButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Log.e("startButton", "clicked");
//                theWebView.loadUrl("https://www.google.com");
//            }
//        });
//
//        refreshButton = findViewById(R.id.refreshButton );
//        refreshButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Log.e("startButton", "clicked");
//                theWebView.loadUrl("https://1mme.com/scanner");
//            }
//        });
//    }
//
//    @Override
//    protected void onResume() {
//        super.onResume();
//
//
//
//    }
//
//    private void tt() {
//        Log.e("tt", "called");
//
//
//    }
}
