
folders=("seq" "omp" "opencilk" "pthreads")

res_name=~/Desktop/results


res=$res_name".txt"

for f in ${folders[@]};do
	cd "scc_"$f
	#make clean
	make
	cd ..
done

graphs=$(find ~/Desktop/graphs/ -name "*.mtx")
 
echo "graph, seq color time, seq pred time, seq total time, seq nscc, omp color time, omp pred time, omp total time, omp nscc, opencilk color time, opencilk pred time, opencilk total time, opencilk nscc, pthreads color time, pthreads pred time, pthreads total time, pthreads nscc" > "$res"

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
