package com.example.vulkaninfoapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Pair;
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
    private List<String> groupList;
    private List<Pair<String, String>> childList;
    private Map<String, List<Pair<String, String>>> mobileCollection;
    private ExpandableListView expandableListView;
    private ExpandableListAdapter expandableListAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

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

    private void createCollection() {

        mobileCollection = new HashMap<String, List<Pair<String, String>>>();
        VkInfo vkInfo = getVkInfo("Vulkan Info App", "No engine");

        for (String group : groupList) {
            switch(group) {
                case "Instance Info":
                    populateInstanceInfo(vkInfo.instanceInfo);
                    break;

                case "Physical Device Properties":
                    populatePhysicalDeviceProperties(vkInfo.physicalDeviceProperties);
                    break;

                case "Physical Device Limits":
                    break;

                case "Physical Device Features":
                    break;

                case "Physical Device Memory Properties":
                    break;

                case "Physical Device Heap Properties":
                    break;

                case "Physical Device Queue Family Properties":
                    break;

                default:
            }

            mobileCollection.put(group, childList);
        }
    }

    private void populatePhysicalDeviceProperties(PhysicalDeviceProperties physicalDeviceProperties) {
        childList = new ArrayList<Pair<String, String>>();
        if (physicalDeviceProperties != null) {
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.apiVersion.ordinal()], String.valueOf(physicalDeviceProperties.apiVersion)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.driverVersion.ordinal()], String.valueOf(physicalDeviceProperties.driverVersion)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.vendorId.ordinal()], String.valueOf(physicalDeviceProperties.vendorId)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.deviceId.ordinal()], String.valueOf(physicalDeviceProperties.deviceId)));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.deviceType.ordinal()], PhysicalDeviceProperties.PhysicalDeviceTypeNames[physicalDeviceProperties.physicalDeviceType.ordinal()]));
            childList.add(new Pair(PhysicalDeviceProperties.PhysicalDevicePropertyNames[PhysicalDevicePropertyIndices.deviceName.ordinal()], String.valueOf(physicalDeviceProperties.deviceName)));
        }
    }

    private void populateInstanceInfo(InstanceInfo instanceInfo) {
        childList = new ArrayList<Pair<String, String>>();
        if (instanceInfo != null) {
            childList.add(new Pair(InstanceInfo.InstancePropertyNames[InstancePropertyIndices.appName.ordinal()], instanceInfo.appName));
            childList.add(new Pair(InstanceInfo.InstancePropertyNames[InstancePropertyIndices.engineName.ordinal()], instanceInfo.engineName));
            childList.add(new Pair(InstanceInfo.InstancePropertyNames[InstancePropertyIndices.numDevices.ordinal()], String.valueOf(instanceInfo.numDevices)));
        }
    }

    /**
     * A native method that is implemented by the 'vulkaninfoapp' native library,
     * which is packaged with this application.
     */
    private native static VkInfo getVkInfo(String appName, String engineName);
}