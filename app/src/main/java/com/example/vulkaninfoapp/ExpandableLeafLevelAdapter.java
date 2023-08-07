package com.example.vulkaninfoapp;

import android.content.Context;
import android.graphics.Typeface;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.TextView;

import java.util.List;
import java.util.Map;

public class ExpandableLeafLevelAdapter extends BaseExpandableListAdapter {

    private Context context;
    private Map<String, List<Pair<String, String>>> leafCollection;
    private List<String> propertyGroupList;

    public ExpandableLeafLevelAdapter(Context context, List<String> propertyGroupList, Map<String, List<Pair<String, String>>> leafCollection) {
        this.context = context;
        this.leafCollection = leafCollection;
        this.propertyGroupList = propertyGroupList;
    }

    @Override
    public int getGroupCount() {
        return leafCollection.size();
    }

    @Override
    public int getChildrenCount(int i) {
        return leafCollection.get(propertyGroupList.get(i)).size();
    }

    @Override
    public Object getGroup(int i) {
        return propertyGroupList.get(i);
    }

    @Override
    public Object getChild(int i, int i1) {
        return leafCollection.get(propertyGroupList.get(i)).get(i1);
    }

    @Override
    public long getGroupId(int i) {
        return i;
    }

    @Override
    public long getChildId(int i, int i1) {
        return i1;
    }

    @Override
    public boolean hasStableIds() {
        return true;
    }

    @Override
    public View getGroupView(int i, boolean b, View view, ViewGroup viewGroup) {
        String groupName = getGroup(i).toString();
        if (view == null) {
            LayoutInflater inflator = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflator.inflate(R.layout.group_item, null);
        }
        TextView item = view.findViewById(R.id.propertyCategory);
        item.setTypeface(null, Typeface.BOLD);
        item.setText(groupName);
        return view;
    }

    @Override
    public View getChildView(int i, int i1, boolean b, View view, ViewGroup viewGroup) {

        Pair<String, String> child = (Pair<String, String>) getChild(i, i1);
        String propertyName = child.first;
        String propertyValue = child.second;

        if (view == null) {
            LayoutInflater inflator = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflator.inflate(R.layout.child_item, null);
        }

        TextView left = view.findViewById(R.id.propertyName);
        left.setText(propertyName);

        TextView right = view.findViewById(R.id.propertyValue);
        right.setText(propertyValue);

        return view;
    }

    @Override
    public boolean isChildSelectable(int i, int i1) {
        return false;
    }
}
