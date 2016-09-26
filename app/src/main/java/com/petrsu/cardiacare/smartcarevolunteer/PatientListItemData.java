package com.petrsu.cardiacare.smartcarevolunteer;

/**
 * Created by nikolay on 11.09.15.
 */
public class PatientListItemData {
    private String title;
    private int imageUrl;
    private String ssPatientUri;

    public PatientListItemData() {}

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getImageUrl() {
        return imageUrl;
    }

    public void setImageUrl(int imageUrl) {
        this.imageUrl = imageUrl;
    }

    public String getSsPatientUri() {
        return ssPatientUri;
    }
    public void setSsPatientUri(String ssPatientUri) {
        this.ssPatientUri = ssPatientUri;
    }
}
