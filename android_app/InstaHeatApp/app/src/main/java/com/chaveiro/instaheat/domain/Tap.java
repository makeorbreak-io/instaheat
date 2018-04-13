package com.chaveiro.instaheat.domain;

public class Tap {
    private final int AUTOMATIC = 0;
    private final int MANUAL = 1;

    private double currentTemp;
    private String name;
    private int state;
    private int preferredTemp;

    public Tap(String name) {
        this.name = name;
        this.preferredTemp = 1;
        // get temperature from arduino
        // get state from arduino
    }

    public boolean changeName(String newName) {
        if(newName.equals(null)) return false;
        if(newName.isEmpty()) return false;
        if(newName.length() > 15) return false;
        name = newName;
        return true;
    }

    public void changeStatus() {
        switch(state) {
            case MANUAL:
                state = AUTOMATIC;
                return;
            case AUTOMATIC:
                state = MANUAL;
                return;
            default:
                return;
        }
    }

    public boolean setStateTo(int newState) {
        if(newState != MANUAL || newState != AUTOMATIC) return false;
        state = newState;
        return true;
    }

    public boolean setPreferredTemp(int newPreferredTemp) {
        if(newPreferredTemp <= 0 || newPreferredTemp >= 100) return false;
        preferredTemp = newPreferredTemp;
        return true;
    }

    public String getName() {
        return this.name;
    }
}
