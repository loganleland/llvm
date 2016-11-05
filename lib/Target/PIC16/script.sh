for f in NOPE*; do
  fnew=`echo $f | sed 's/NOPE/PIC16/'`
  mv $f $fnew
done
