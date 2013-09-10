#!/bin/sh

rm -rf ./libs
curl http://s3.amazonaws.com/eegeo-static/sdk.package.android.tar.gz > ./sdk.package.tar.gz
tar -zxvf ./sdk.package.tar.gz
rm -f ./sdk.package.tar.gz
value=`cat ./sdk.package.android/version.txt`
echo "Platform version --> $value"
mv ./sdk.package.android/ ./libs