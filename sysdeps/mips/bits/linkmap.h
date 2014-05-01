struct link_map_machine
  {
    ElfW(Addr) plt; /* Address of .plt */
    ElfW(Word) fpabi; /* FP ABI of the object */
  };
