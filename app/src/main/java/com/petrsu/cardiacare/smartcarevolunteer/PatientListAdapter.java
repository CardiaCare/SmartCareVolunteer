package com.petrsu.cardiacare.smartcarevolunteer;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Collections;
import java.util.List;
import java.util.ResourceBundle;

/**
 * Created by nikolay on 11.09.15.
 */
public class PatientListAdapter extends RecyclerView.Adapter<PatientListAdapter.ViewHolder> {

    protected List<PatientListItemData> itemsData = Collections.emptyList();

    public PatientListAdapter(List<PatientListItemData> mData) {
       itemsData = mData;
    }
    public void updateList(List<PatientListItemData> data) {
        itemsData = data;
        notifyDataSetChanged();
    }
    public PatientListAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        // create a new view
        View itemLayoutView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_layout, null);

        // create ViewHolder

        ViewHolder viewHolder = new ViewHolder(itemLayoutView);
        return viewHolder;
    }
    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(ViewHolder viewHolder, int position) {

        // - get data from your itemsData at this position
        // - replace the contents of the view with that itemsData

        viewHolder.txtViewTitle.setText(itemsData.get(position).getTitle());
        viewHolder.imgViewIcon.setImageResource(itemsData.get(position).getImageUrl());
    }
    @Override
    public int getItemCount() {
        return itemsData.size();
    }

    /*
     *
     * ViewHolder
     *
     */
    public static class ViewHolder extends RecyclerView.ViewHolder{

        public TextView txtViewTitle;
        public ImageView imgViewIcon;

        public ViewHolder(View itemLayoutView) {
            super(itemLayoutView);
            txtViewTitle = (TextView) itemLayoutView.findViewById(R.id.item_title);
            imgViewIcon = (ImageView) itemLayoutView.findViewById(R.id.item_icon);
        }
    }
    // Return the size of your itemsData (invoked by the layout manager)

//    public void addItem(int position, PatientListItemData data) {
//        itemsData.add(position, data);
//        notifyItemInserted(position);
//    }
//
//    public void removeItem(int position) {
//        itemsData.remove(position);
//        notifyItemRemoved(position);
//    }
}
