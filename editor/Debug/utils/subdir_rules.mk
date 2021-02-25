################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
utils/uartstdio.obj: ../utils/uartstdio.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.8/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.8/include" --include_path="C:/ti/TivaWare" --define=ccs="ccs" --define=PART_TM4C123GH6PGE --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="utils/uartstdio.pp" --obj_directory="utils" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


