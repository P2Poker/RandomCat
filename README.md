# RandomCat
Multi-threaded, feature-rich, cryptographic true random number generator.
Swift, agile, and responsive; like a cat. :3
Uses AES to generate random blocks, and SHA to incorporate entropy.
Entropy bytes will be harvested from multiple locations: user-input timestamps, network transmission timestamps, the internal OS secure random generator, etc.
