#!/bin/bash



time=$(date "+%Y-%m-%d-%H:%M:%S")
echo "${time}"
mkdir ${time} 
echo "创建完成"

cp ../../properties/property.json ${time}

mv *.db ${time}
mv *.db-journal ${time}
cp outputload.sh ${time}
cp *.py ${time}
echo "复制完成"
cd ${time}
./outputload.sh
mkdir csv
mkdir database
mv *.csv csv
mv *.py csv
mv *.db database
cd csv
./plotXY.py
cd ..

echo "请输入该case描述："
cat - > ${time}.log





