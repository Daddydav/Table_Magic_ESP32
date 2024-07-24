void init_quetes(){
    Quetes[0].rune[0] = true;
    std::copy_n(NEGATION, 4, Quetes[0].sort[0]);
    Quetes[0].rune[1] = true;
    std::copy_n(CONTROLE, 4, Quetes[0].sort[1]);
    Quetes[0].rune[2] = true;
    std::copy_n(ENERGIE, 4, Quetes[0].sort[2]);
    Quetes[0].rune[3] = true;
    std::copy_n(AIR, 4, Quetes[0].sort[3]);
    Quetes[0].rune[4] = false;
    std::copy_n(DUMY, 4, Quetes[0].sort[4]);
    
}