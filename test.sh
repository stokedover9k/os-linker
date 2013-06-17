for file in inout/input-*
do
  echo "======== ${file} ========"
  basefile=`basename ${file}`
  outfile=${basefile/#input/out}
  ./linker ${file} > out/${outfile}
  diff -b -B -E out/${outfile} inout/${outfile}
done
