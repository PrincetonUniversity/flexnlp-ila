#simple makefile
#MACROS
CC = g++
CFLAGS = -std=c++11 -g

all: json_gen extract_input flexnlp_axi_rd_commands_gen instr_template_gen
clean: 
	rm -f *.out *.o

json_gen: prog_frag_gen.cc
	$(CC) $(CFLAGS) prog_frag_gen.cc -o json_gen.out

extract_input: extract_test_input.cc
	$(CC) $(CFLAGS) extract_test_input.cc -o extract_test_input.out

flexnlp_axi_rd_commands_gen: flexnlp_axi_rd_commands_gen.cc
	$(CC) $(CFLAGS) flexnlp_axi_rd_commands_gen.cc -o flexnlp_axi_rd_commands_gen.out

instr_template_gen: instr_template_gen.cc
	$(CC) $(CFLAGS) instr_template_gen.cc -o instr_template_gen.out

