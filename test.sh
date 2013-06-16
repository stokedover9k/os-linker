for file in inout/input-*
do
  echo "======== ${file} ========"
  ./linker $file
done
