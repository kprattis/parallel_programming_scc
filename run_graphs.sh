#=================================================
folders=("seq", "omp", "opencilk", "pthreads")

res_name=~/Desktop/results
graph_folder=~/Desktop/graphs/

#=================================================
graphs=$(find $graph_folder -name "*.mtx")
#=================================================


res=$res_name".txt"

for f in ${folders[@]};do
	cd "scc_"$f
	#make clean
	make
	cd ..
done



echo -n "graph" > $res

for f in ${folders[@]};do
	echo -n ", $f color time, $f pred time, $f total time, $f nscc" >> $res
done

echo "" >> $res

for g in ${graphs[@]};do
	
	name=$(basename $g ".mtx")
	
	echo "Now processing graph $name"
	echo -n "$name, " >> $res
	
	for f in ${folders[@]};do
		cd "scc_"$f
		bin/$f"_scc" $g >> "$res"
		cd ..
	done
	
	echo "" >> $res
done

mv "$res_name.txt" "$res_name.csv"
