package com.tomagoyaky.Attacker;

import java.util.ArrayList;
import java.util.List;

import cn.trinea.android.common.util.ShellUtils;
import cn.trinea.android.common.util.ShellUtils.CommandResult;

public class MemSearch {

	public static String dowork(String data, int pid){

		List<String> commnandList = new ArrayList<String>();
		commnandList.add("cat /proc/" + pid + "/maps");
		CommandResult result = ShellUtils.execCommand(commnandList, true);
		
		// 解析maps并找到应用对应的内存空间
		String[] mapLineArray = result.successMsg.split("\n");
		for (String mapLine : mapLineArray) {
			String[] mapArray = mapLine.split(" ");
			ArrayList<String> mapList = new ArrayList<String>();
			for (String mapItem : mapArray) {
				if(!mapItem.equals(""))
					mapList.add(mapItem);
			}
			
			if(mapList.size() == 6){
				String keyName = mapList.get(mapList.size() - 1);
				String[] addressArray = mapList.get(0).split("-");
				long keyAddressStart = Long.parseLong(addressArray[0]);
				long keyAddressEnd = Long.parseLong(addressArray[1]);
				
			}
			int a = 0;
		}
		
		commnandList.clear();
		commnandList.add("chmod 777 /data/local/tmp/tomagoyaky_server");
		commnandList.add("/data/local/tmp/tomagoyaky_server " + " -n null" + " -p " + pid + " -d " + data + " -s 1 1002");
		result = ShellUtils.execCommand(commnandList, true);

		if(result.successMsg.equals(""))
			return result.errorMsg;

		if(result.errorMsg.equals(""))
			return result.successMsg;
		return "";
	}
}
