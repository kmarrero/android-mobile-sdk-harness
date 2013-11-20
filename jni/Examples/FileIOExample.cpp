//
//  FileIOExample.cpp
//  ExampleApp
//
//  Created by eeGeo on 30/04/2013.
//  Copyright (c) 2013 eeGeo. All rights reserved.
//

#include "FileIOExample.h"
#include <sstream>
#include <vector>
#include "Logger.h"

using namespace Eegeo::Helpers;

namespace Examples
{
    FileIOExample::FileIOExample(IFileIO& fileIO)
    :fileIO(fileIO)
    {
    }
    
    void FileIOExample::Start()
    {
        srand(time(0));
        Byte data[] = { 1, 2, 3, 4, 5 };
        
        std::stringstream ss;
        ss << "mydata_" << rand() << ".bytes";
        std::string filename = ss.str();
        
        bool exists = fileIO.Exists(filename);
        
        EXAMPLE_LOG("%s %s\n", filename.c_str(), exists ? "exists" : "does not exist");
        if(!fileIO.WriteFile(data, 5ul, filename))
        {
        	EXAMPLE_LOG("Failed to write to filesystem!\n");
            return;
        }
        
        exists = fileIO.Exists(filename);
        EXAMPLE_LOG("%s now %s\n", filename.c_str(), exists ? "exists" : "does not exist");
        
        std::fstream stream;
        size_t size;
        if(fileIO.OpenFile(stream, size, filename))
        {
        	EXAMPLE_LOG("Opened File!\n");
            
            std::vector<Byte> readBackData;
            readBackData.resize(size);
            stream.read((char*)&readBackData[0], size);
            
            for(int i = 0; i < size; ++ i)  {
            	EXAMPLE_LOG("Read %d at position %d\n", readBackData[i], i);
            }
        }
        
        stream.close();
        EXAMPLE_LOG("Trying to delete %s...\n", filename.c_str());
        bool deleted = fileIO.DeleteFile(filename);
        EXAMPLE_LOG("Deleting %s %s!\n", filename.c_str(), deleted ? "succeeded" : "failed");
        
        //load app data from nominated app data directories - these asset directories were
        //nominated at construction of the fileIO object in AppWindow.cpp
        std::vector<std::string> appAssets;
        appAssets.push_back("MyAppDataDirectory/MySubDirectory/MyData1.txt");
        appAssets.push_back("MyAppDataDirectory/MyData1.txt");
        appAssets.push_back("MyAppDataDirectory/MyData2.txt");

        for(std::vector<std::string>::const_iterator it = appAssets.begin(); it != appAssets.end(); ++ it)
        {
        	const std::string& appAsset = *it;

            std::fstream customAppDataStream; size_t customAppDataSize;
			if(fileIO.OpenFile(customAppDataStream, customAppDataSize, appAsset))
			{
				std::string fileContent((std::istreambuf_iterator<char>(customAppDataStream)), std::istreambuf_iterator<char>());
				EXAMPLE_LOG("File %s content --> %s...\n", appAsset.c_str(), fileContent.c_str());
			}
			else
			{
				EXAMPLE_LOG("Failed to open file %s!\n", appAsset.c_str());
			}
			customAppDataStream.close();
        }

        EXAMPLE_LOG("Done!\n");
    }
}
