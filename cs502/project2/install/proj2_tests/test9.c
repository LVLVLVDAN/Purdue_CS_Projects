int glob1;

void init()
{
   glob1 = 1;
}
main()
{
  int k=1;


init();
if ( k > 0 ) glob1 = 0;


printf("glob1=%d\n",glob1);
}

