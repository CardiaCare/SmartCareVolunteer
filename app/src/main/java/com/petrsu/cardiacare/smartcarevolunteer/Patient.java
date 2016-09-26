package com.petrsu.cardiacare.smartcarevolunteer;

/**
 * Created by nikolay on 23.09.15.
 */
public class Patient {
    public String uri;
    public String name;
    public String age;
    public String img;
    public String phone;

    public Patient(String uri, String name, String age, String img, String phone) {
        this.uri = uri;
        this.name = name;
        this.age = age;
        this.img = img;
        this.phone = phone;
    }
}
