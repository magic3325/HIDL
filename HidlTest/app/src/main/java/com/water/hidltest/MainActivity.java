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
    private Button mOnBtn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mText =(TextView) findViewById(R.id.text);
        mEdit =(EditText) findViewById(R.id.edit);
        mBtn =(Button) findViewById(R.id.button);

        //获取hidl服务
        try {
            mITestServer =ITest.getService();
        } catch (RemoteException e) {
            e.printStackTrace();
        }

    }
    public void hidlTest(View view){
        if (mITestServer != null){
            Log.d("gunder", "service is connect.");
            String text = null;
            //访问hidl接口
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

    public void hidlOn(View view){
        if (mITestServer != null){
            Log.d("gunder", "service is connect.");
            int status=-1;
            //访问hidl接口




            try {
                int time = Integer.parseInt(mEdit.getText().toString());
                 status = mITestServer.on(time);//调用HAL层接口
            } catch (RemoteException e) {
                e.printStackTrace();

            }catch (NumberFormatException e) {
                e.printStackTrace();
                Toast.makeText(this, "请输入正确数字", Toast.LENGTH_LONG).show();
            }

            mText.setText(String.valueOf(status));
            Toast.makeText(this, String.valueOf(status), Toast.LENGTH_LONG).show();
        }
    }

    public void hidlOff(View view){
        if (mITestServer != null){
            Log.d("gunder", "service is connect.");
            int status=-1;
            //访问hidl接口
            try {
                status = mITestServer.off();//调用HAL层接口
            } catch (RemoteException e) {
                e.printStackTrace();
            }

            mText.setText(String.valueOf(status));
            Toast.makeText(this, String.valueOf(status), Toast.LENGTH_LONG).show();
        }
    }


}
