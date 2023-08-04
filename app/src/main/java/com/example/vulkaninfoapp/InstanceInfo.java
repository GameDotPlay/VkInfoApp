package com.example.vulkaninfoapp;

public class InstanceInfo {

    public String appName;
    public String engineName;
    public long numExtensions;
    public ExtensionProperties[] availableExtensions;

    public long numDevices;

    public static final String[] InstancePropertyNames = {
            "Application name",
            "Engine name",
            "Number of Available Extensions",
            "Available Extensions",
            "Number of devices"
    };
}
