
folders=("seq" "omp" "opencilk" "pthreads")

results = "~/Desktop/results.txt"

for f in ${folders[@]};do
	cd "scc_"$f
	#make clean
	make
	cd ..
done

echo "" > $results

for b in ${folders[@]};do
	cd "scc_"$b
	find ~/Desktop/graphs/ -name "*.mtx" |xargs -I {} bin/$b"_scc" {} >> $results 
	cd ..
done
