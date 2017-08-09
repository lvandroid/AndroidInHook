package com.andr0day.androidinlinehook;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.List;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import okhttp3.Dns;

public class MainActivity extends AppCompatActivity {
    private Executor executor = Executors.newSingleThreadExecutor();

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                stringFromJNI();
            }
        });
        findViewById(R.id.http).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                executor.execute(new Runnable() {
                    @Override
                    public void run() {
                        NetUtils.okhttpRequest();
//                        NetUtils.get("https://leetcode.com/problems/two-sum/?tab=Description");
//                        NetUtils.get("http://www.baidu.com");
                        Dns dns = new Dns() {
                            @Override
                            public List<InetAddress> lookup(String hostname) throws UnknownHostException {
                                Log.d("JNIHOOK",hostname);
                                return null;
                            }
                        };

                        try {
                            new Socket("www.baidu.com",8888);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
            }
        });
        findViewById(R.id.unhook).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                unhook();
            }
        });

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void unhook();
}
