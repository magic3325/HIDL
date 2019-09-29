package com.water.hidltest;

import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import vendor.tinno.hardware.test.V1_0.ITest;

public class MainActivity extends AppCompatActivity {

    ITest mITestServer;
    private TextView mText;
    private EditText mEdit;
    private Button mBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mText =(TextView) findViewById(R.id.text);
        mEdit =(EditText) findViewById(R.id.edit);
        mBtn =(Button) findViewById(R.id.button);

        try {
            mITestServer =ITest.getService(); //获取服务
        } catch (RemoteException e) {
            e.printStackTrace();
        }

    }
    public void hidlTest(View view){
        if (mITestServer != null){
            Log.d("gunder", "service is connect.");
            String text = null;
            try {
                text = mITestServer.helloWorld(mEdit.getText().toString());//调用HAL层接口
            } catch (RemoteException e) {
                e.printStackTrace();
            }
            Log.d("gunder", text);
            mText.setText(text);
            Toast.makeText(this, text, Toast.LENGTH_LONG).show();
        }
    }

}
