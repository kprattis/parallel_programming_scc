#=================================================
folders=("omp" "opencilk" "pthreads")

res_name=~/Desktop/results-sweep-num-threads
graph_folder=~/Desktop/graphs/

nthreads=(6, 8, 10, 12, 14, 16, 18, 25, 50)

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

for n in ${nthreads[@]};do
	for f in ${folders[@]};do
		echo -n ", $f $n threads color time, $f $n threads pred time, $f $n threads total time, $f $n threads nscc" >> $res
	done
done

echo "" >> $res

for g in ${graphs[@]};do

	name=$(basename $g ".mtx")
	echo "Now processing graph $name"		
	echo -n "$name, " >> $res
	
	for n in ${nthreads[@]};do
		cd scc_omp
		bin/omp_scc $g $n >> "$res"
		cd ..
		cd scc_opencilk
		CILK_NWORKERS=$n bin/opencilk_scc $g >> "$res"
		cd ..
		cd scc_pthreads
		bin/pthreads_scc $g $n >> "$res"
		cd ..
	done
	echo "" >> $res
done

mv "$res_name.txt" "$res_name.csv"
