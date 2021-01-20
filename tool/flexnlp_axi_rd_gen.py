import sys

FLEXNLP_VECTOR_SIZE = 16
FLEXNLP_GBCORE_NUM_BANKS = 16
FLEXNLP_GB_LARGE_BUF_BASE = '0x33500000'

def get_addr(mem_base, ts_num, v_num, ts_idx, v_idx):
  timestep_size = v_num * FLEXNLP_VECTOR_SIZE
  group_size = timestep_size * FLEXNLP_GBCORE_NUM_BANKS
  gb_buf_row_size = FLEXNLP_GBCORE_NUM_BANKS * FLEXNLP_VECTOR_SIZE
  out = (int(ts_idx/FLEXNLP_GBCORE_NUM_BANKS)) * group_size + \
        (ts_idx%FLEXNLP_GBCORE_NUM_BANKS) * FLEXNLP_VECTOR_SIZE + \
        gb_buf_row_size * v_idx
  out += int(FLEXNLP_GB_LARGE_BUF_BASE, base=16)
  out += int(mem_base, base=16)
  return hex(out)

def produce_test_with_rds(fin, out_path, mem_base, ts_num, v_num):
  with open(out_path, 'w') as fout:
    fout.write(fin.read())
    fout.write('100,Q,0x0,0x0\n')
    for ts in range(ts_num):
      for v in range(v_num):
        addr = get_addr(mem_base, ts_num, v_num, ts, v)
        fout.write('2,R,' + addr + ',0x0\n')


if __name__ == "__main__":
  assert len(sys.argv) == 5, "wrong input arguments\n" + \
    "Usage: python3 flexnlp_axi_rd_gen.py [testbench] [mem_base] [out_ts_num] [out_v_num]"
  
  file_path = sys.argv[1]
  out_path = file_path[:-4] + '_with_rd.csv'
  with open(file_path, 'r') as fin:
    produce_test_with_rds(fin, out_path, sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))
  