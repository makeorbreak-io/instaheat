package com.chaveiro.instaheat.domain;

public class Utils {

    public static double CelsiusToKelvin(double celsius){
        return (274.15 + celsius);
    }

    public static double CelsiusToFahrenheit(double celsius){
        return ((1.8*celsius)+ 32);
    }
}
