echo "Compilando o programa..."

gcc -pedantic -g ./src/main.c ./src/avl.c ./include/avl.h ./src/hash.c ./include/hash.h -lm

echo "Executando o programa..."

./a.out

echo "Limpeza dos arquivos gerados..."

rm -r ./a.out.dSYM
rm -r ./include/avl.h.gch
rm -r ./include/hash.h.gch