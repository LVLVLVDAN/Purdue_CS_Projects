
int main()
{
  int i = 1;
  int a1, a2, a3, a4;

  if (i ==1) {
    printf("now i = 1\n");
    a1 = 10;
    return i+a1;
  }else {
    printf("now i!=1\n");
    if (i == 2) {
      a1 = 10;
      a2 = 10;
      printf ("now i = 2\n");
      return a1+a2+i;
    }
  }
  a1 = 10;
  printf("%d %d %d \n", a1, a2, a3);
  return i;
}
