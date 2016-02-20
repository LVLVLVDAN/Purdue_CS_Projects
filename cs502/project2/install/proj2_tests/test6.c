main()
{
    float   x, y;

    x = 6;

	while (x>0)
	{
        if (x > 5.0) y = 4.0;
	    else break;
           x = x - y;
	}

    printf("Values AFTER %f, %f\n", x, y);
    return 0;
}
