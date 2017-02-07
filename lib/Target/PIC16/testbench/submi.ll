; ModuleID = 'submi.c'
source_filename = "submi.c"
target datalayout = "e-m:e-p:8:8-i8:8:8-a:8-n8"
target triple = "pic16"

; Function Attrs: nounwind
define i8 @main() #0 {
entry:
  %retval = alloca i8, align 1
  %alpha = alloca i8, align 1
  %beta = alloca i8, align 1
  store i8 0, i8* %retval, align 1
  store i8 42, i8* %alpha, align 1
  store i8 32, i8* %beta, align 1
  %0 = load i8, i8* %beta, align 1
  %1 = load i8, i8* %alpha, align 1
  %sub = sub nsw i8 %1, %0
  store i8 %sub, i8* %alpha, align 1
  ret i8 0
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.0 (tags/RELEASE_390/final)"}
