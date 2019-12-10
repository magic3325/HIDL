# HIDL


# HidlTest apk ,主要是java 层如何调用hild借口

ａｐｋ要调用hidl，就必须添加hidl .java库，vendor.tinno.hardware.test-V1.0-java.jar。这个是由Itest.hal 编译生成的，也可自己写android.bp文件，手动编译生成，后边会具体介绍。

1.添加jar包，需要添加hidl接口编译出来的包vendor.tinno.hardware.test-V1.0-java.jar，和平台对应的framework.jar；


```

├── libs
│   │   ├── framework.jar
│   │   └── vendor.tinno.hardware.test-V1.0-java.jar


　dependencies {
    compileOnly files('libs/framework.jar')
    implementation files('libs/vendor.tinno.hardware.test-V1.0-java.jar')
}
```
2.添加签名，因为hidl 是系统平台级接口，需要对应的平台签名才能有访问权限，platform_keystore　为系统签名文件，需要keytool　通过platform.pk8　生成platform.keystore
```
signingConfigs {
        debug {
        }
        release {
            storeFile file("./platform_keystore")
            storePassword "android"
            keyAlias "platform"
            keyPassword "android"
        }
    }
```

３，java代码如何访问；

```
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

}

```
获取hidl服务
```
try {
            mITestServer =ITest.getService(); //获取服务
        } catch (RemoteException e) {
            e.printStackTrace();
        }
```
访问服务借口；
```
 try {
                text = mITestServer.helloWorld(mEdit.getText().toString());//调用HAL层接口
            } catch (RemoteException e) {
                e.printStackTrace();
            }
```


