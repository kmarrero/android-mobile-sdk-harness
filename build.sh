#!/bin/sh

rm -rf "./libs"
rm -rf "./obj"
rm -rf "./bin"

sh "update.platform.sh"

ndk-build
resultcode=$?

echo
if [ $resultcode = 0 ] ; then
  echo "COMPILE ANDROID PROJECT SUCCEEDED"
else
  echo "COMPILE ANDROID PROJECT FAILED"
fi


exit $resultcode
