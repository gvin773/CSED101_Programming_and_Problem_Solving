#include <stdio.h>

char calc_grade(float a);

int main()
{
	FILE *infile, *outfile;
	char ch;
	int name, mid, fin, num;
	double sum = 0;
	
	infile = fopen("score.txt", "r");
	outfile = fopen("report.txt", "w");
	
	fprintf(outfile, "==============================\n");
	fprintf(outfile, "  학번\t\t\t총점(학점)\n");
	fprintf(outfile, "==============================\n");
	
	for(num = 0; fscanf(infile, "%d %d %d", &name, &mid, &fin) != EOF; num++)
	{
		fprintf(outfile, "%d\t\t\t%.1lf (%c)\n", name, (double)mid*0.4+(double)fin*0.6, calc_grade((double)mid*0.4+(double)fin*0.6));
		sum += (double)mid*0.4+(double)fin*0.6;
	}
	
	fprintf(outfile, "==============================\n");
	fprintf(outfile, "  평균\t\t\t%.1lf\n", sum/num);
	fprintf(outfile, "==============================");
	
	fclose(outfile);
	fclose(infile);
	
	return 0;
}

char calc_grade(float a)
{
	if(a >= 90)
		return 'A';
	
	if(a >= 80)
		return 'B';
	
	if(a >= 70)
		return 'C';
	
	if(a >= 60)
		return 'D';
	
	if(a < 60)
		return 'F';
}
