package com.eegeo;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceView;

public class EegeoSurfaceView extends SurfaceView 
{
	private MainActivity m_mainActivity;
	
	public EegeoSurfaceView(Context context, AttributeSet attrs) 
	{
		super(context, attrs);
	}
	
	public void setMainActivity(MainActivity mainActivity)
	{
		m_mainActivity = mainActivity;
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent e) {

        m_mainActivity.processTouchEvent(e);
	    return true;
	}
}
