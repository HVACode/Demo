#!/bin/bash
#!/bin/bash
filename=""
filesDir="FileResource"
function menu {
	clear
	echo
	echo -e "\t\t\tSys Admin Menu\n"
	echo -e "\t1. Choose file for query"
	echo -e "\t2. Query"
	echo -e "\t3. Quit"

	if [ ${#filename} -ne 0 ]; then
		echo "query file for the moment: " $filename
	fi

	read -p "[Enter your option]: " option
}

function ChooseQueryFile {
	# 输出文件列表
	while [ 1 ]; do
		if [ ${#filename} -ne 0 ]; then
			echo "query file for the moment: " $filename
		fi

		index=0
		array=()

		for file in $filesDir/*; do
			if test -f $file; then
				echo $(($index + 1))". "$file
				array[++index]=$file
				#index = $(($index + 1))
			fi
		done

		# 读取选择
		read -n1 -p "[Name the file]:" filenameIdx
		echo
		filename=${array[$filenameIdx]}
		echo "file '"$filename"' has been selected"

		read -n1 -p "[Are you sure(y for yes)]:" doubleCheck

		if [ $doubleCheck = "y" ]; then
			break
		fi
	done
}

function Query {
	# 若文件未选择则不应该继续执行
	if [ ${#filename} -eq 0 ]; then
		echo "you haven't chosen file"
		return
	fi
	echo "File :${filename}"
	read -p "[query statement]:" query
	# 拆分
	oldIFS="$IFS"
	IFS=" "
	args=($query)
	IFS="$oldIFS"

	#根据参数个数调用可执行程序
	if [ ${#args[@]} -eq 1 ]; then
		./query $filename ${args[0]}
	elif [ ${#args[@]} -eq 2 ]; then
		./query $filename ${args[0]} ${args[1]}
	elif [ ${#args[@]} -eq 3 ]; then
		./query $filename ${args[0]} ${args[1]} ${args[2]}
	fi
}

while [ 1 ]; do
	menu
	case $option in
	1)
		ChooseQueryFile
		;;
	2)
		Query
		read -n1 -p "Continue( y for yes )" t
		if [ $t != "y" ]; then
			break
		fi
		;;
	3)
		break
		;;
	*)
		clear
		echo "sorry,invalid selection"
		sleep 1
		;;
	esac
done
