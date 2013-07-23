package com.eegeo.ui.nativeinput;

import android.app.Activity;
//import android.app.AlertDialog;
import android.content.Context;
//import android.content.DialogInterface;
import android.text.InputType;
import android.util.Log;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

public class KeyboardInput
{
	public static String text()
	{
		return ms_text;
	}

	private static String ms_text = "";
	private static EditText m_editText;
	private static InputMethodManager m_inputManager;

	public native static void dismissCallback(int ptr);
	public native static void keyPressCallback(int ptr);
	
	public static void dismiss()
	{
		m_inputManager.hideSoftInputFromWindow(m_editText.getWindowToken(), 0);
		m_editText = null;
	}

	public static void showKeyboard(final Activity a, final int keyboardType, final int returnKeyType, final int ptr)
    {
    	a.runOnUiThread(new Runnable()
    	{
    		public void run()
    		{
    	    	try
    	    	{
    	    		m_editText = new EditText(a);
    	    		m_editText.setInputType(InputType.TYPE_CLASS_TEXT);
    	    		
    	    		m_inputManager = (InputMethodManager)a.getSystemService(Context.INPUT_METHOD_SERVICE);
    	    		m_inputManager.showSoftInput(m_editText, InputMethodManager.SHOW_IMPLICIT);
    	    	}
    	    	catch (Exception e)
    	    	{
    	        	Log.v("KeyboardInput", e.getMessage() == null ? "Error, but no message?!" : e.getMessage());
    	    	}
    		}
    	});
    }
}
