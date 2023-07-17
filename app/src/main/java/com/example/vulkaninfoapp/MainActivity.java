package com.example.vulkaninfoapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.Toast;

import com.example.vulkaninfoapp.databinding.ActivityMainBinding;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'vulkaninfoapp' library on application startup.
    static {
        System.loadLibrary("vulkaninfoapp");
    }

    private ActivityMainBinding binding;

    List<String> groupList;
    List<String> childList;
    Map<String, List<String>> mobileCollection;
    ExpandableListView expandableListView;
    ExpandableListAdapter expandableListAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        String appName = initInstance("TestName", "TestEngine");
        createGroupList();
        createCollection();
        expandableListView = findViewById(R.id.vulkanInfo);
        expandableListAdapter = new MyExpandableListAdapter(this, groupList, mobileCollection);
        expandableListView.setAdapter(expandableListAdapter);
        expandableListView.setOnGroupExpandListener(new ExpandableListView.OnGroupExpandListener() {
            int lastExpandedPosition = -1;

            @Override
            public void onGroupExpand(int i) {
                if (lastExpandedPosition != -1 && i != lastExpandedPosition) {
                    expandableListView.collapseGroup(lastExpandedPosition);
                }
            }
        });
        expandableListView.setOnChildClickListener(new ExpandableListView.OnChildClickListener() {
            @Override
            public boolean onChildClick(ExpandableListView expandableListView, View view, int i, int i1, long l) {
                String selected = expandableListAdapter.getChild(i, i1).toString();
                Toast.makeText(getApplicationContext(), "Selected: " + selected, Toast.LENGTH_SHORT).show();
                return true;
            }
        });

        //TextView tv = binding.sampleText;
        //tv.setText(stringFromJNI());
    }

    private void createCollection() {
        String[] samsungModels = {"Samsung Galaxy M21", "Samsung Galaxy F41", "Samsung Galaxy M51", "Samsung Galaxy A50s"};
        String[] googleModels = {"Pixel 4 XL", "Pixel 3a", "Pixel 3 XL", "Pixel 3a XL", "Pixel 2", "Pixel 3"};
        String[] redmiModels = {"Redmi 9i", "Redmi Note 9 Pro Max", "Redmi Note 9 Pro"};
        String[] vivoModels = {"Vivo V20", "Vivo S1 Pro", "Vivo Y91i", "Vivo Y12"};
        String[] nokiaModels = {"Nokia 5.3", "Nokia 2.3", "Nokia 3.1 Plus"};

        mobileCollection = new HashMap<String, List<String>>();

        for (String group : groupList) {
            if (group.equals("Instance Info")) {
                loadChild(samsungModels);
            } else if (group.equals("Physical Device Properties")) {
                loadChild(googleModels);
            } else if (group.equals("Physical Device Limits")) {
                loadChild(googleModels);
            } else if (group.equals("Physical Device Features")) {
                loadChild(redmiModels);
            } else if (group.equals("Physical Device Memory Properties")) {
                loadChild(vivoModels);
            }else if (group.equals("Physical Device Memory Heap Properties")) {
                    loadChild(vivoModels);
            } else if (group.equals("Physical Device Queue Family Properties")) {
                loadChild(nokiaModels);
            }

            mobileCollection.put(group, childList);
        }
    }

    private void loadChild(String[] properties) {
        childList = new ArrayList<>();
        for (String property : properties) {
            childList.add(property);
        }
    }

    private void populateInstanceInfo() {
        childList = new ArrayList<>();
    }

    private void createGroupList() {
        groupList = new ArrayList<>();
        groupList.add("Instance Info");
        groupList.add("Physical Device Properties");
        groupList.add("Physical Device Limits");
        groupList.add("Physical Device Sparse Properties");
        groupList.add("Physical Device Features");
        groupList.add("Physical Device Memory Properties");
        groupList.add("Physical Device Memory Heap Properties");
        groupList.add("Physical Device Queue Family Properties");
    }

    /**
     * A native method that is implemented by the 'vulkaninfoapp' native library,
     * which is packaged with this application.
     */
    public native static String initInstance(String appName, String engineName);
}