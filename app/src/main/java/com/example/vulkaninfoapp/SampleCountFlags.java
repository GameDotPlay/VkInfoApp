package com.example.vulkaninfoapp;

public enum SampleCountFlags {
    SampleCount1Bit("1 sample per pixel"),
    SampleCount2Bit("2 samples per pixel"),
    SampleCount4Bit("4 samples per pixel"),
    SampleCount8Bit("8 samples per pixel"),
    SampleCount16Bit("16 samples per pixel"),
    SampleCount32Bit("32 samples per pixel"),
    SampleCount64Bit("64 samples per pixel");

    public final String description;

    private SampleCountFlags(String description) {
        this.description = description;
    }
}
