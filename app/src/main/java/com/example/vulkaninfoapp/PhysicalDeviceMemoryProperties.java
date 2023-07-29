package com.example.vulkaninfoapp;

public class PhysicalDeviceMemoryProperties {
    public long memoryTypeCount;
    public MemoryType[] memoryTypes;
    public long memoryHeapCount;

    public MemoryHeap[] memoryHeaps;

    public static final String[] PhysicalDeviceMemoryPropertyNames = {
            "Memory type count",
            "Memory heap",
            "Memory heap count",
            "Memory heaps"
    };
}
