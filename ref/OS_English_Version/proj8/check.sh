#cat diff_two_file
#/bin/sbin
file1=./correct.txt
file2=./out.txt
diff $file1 $file2 > /dev/null
if [ $0 == 0 ]; then
    echo "Both file are same"
else
    echo "Both file are different"
fi
