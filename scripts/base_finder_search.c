#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "generator.h"
#include "biomes.h"

#define MAX_RESULTS 50000
#define MAX_REGIONS 500
typedef struct { int x,z; } Region;
typedef struct { int x,z; float y,plains_pct,relief,avg_deviation; int mountain_x,mountain_z; float mountain_y,rise,distance,mass,score; } Result;
static Generator g; static SurfaceNoise sn;
static void terrain(int x,int z,float*y,int*id){ mapApproxHeight(y,id,&g,&sn,x>>2,z>>2,1,1); }
static int is_plains(int id){ return id==plains || id==sunflower_plains; }
static int cmp(const void*a,const void*b){ float d=((const Result*)b)->score-((const Result*)a)->score; return (d>0)-(d<0); }

static int analyse(int x,int z,Result*r){
 float cy; int cid; terrain(x,z,&cy,&cid); if(!is_plains(cid)) return 0;
 int plains_n=0,total=0; float miny=9999,maxy=-9999,sum=0;
 for(int dz=-112;dz<=112;dz+=28) for(int dx=-112;dx<=112;dx+=28){ float y;int id;terrain(x+dx,z+dz,&y,&id);total++;if(is_plains(id))plains_n++;if(y<miny)miny=y;if(y>maxy)maxy=y;sum+=y; }
 float avg=sum/total, plains_pct=100.0f*plains_n/total, relief=maxy-miny, devsum=0;
 for(int dz=-112;dz<=112;dz+=28) for(int dx=-112;dx<=112;dx+=28){ float y;int id;terrain(x+dx,z+dz,&y,&id);devsum+=fabsf(y-avg); }
 float avgdev=devsum/total; if(plains_pct<50.0f) return 0;
 float best_score=-99999,best_y=-9999,best_dist=9999; int best_x=0,best_z=0,high=0,mtot=0;
 for(int dz=-800;dz<=800;dz+=32) for(int dx=-800;dx<=800;dx+=32){ float dist=sqrtf((float)(dx*dx+dz*dz));if(dist<160||dist>800)continue;float y;int id;terrain(x+dx,z+dz,&y,&id);mtot++;float rise=y-avg;if(rise>=40)high++;float target=rise-dist*0.035f;if(target>best_score){best_score=target;best_y=y;best_dist=dist;best_x=x+dx;best_z=z+dz;} }
 float rise=best_y-avg,mass=mtot?100.0f*high/mtot:0;if(rise<45)return 0;
 float flat=15.0f-avgdev;if(flat<0)flat=0;float prox=800.0f-best_dist;if(prox<0)prox=0;
 float score=plains_pct*0.40f+flat*2.0f+rise*0.30f+mass*0.80f+prox*0.025f;
 *r=(Result){x,z,avg,plains_pct,relief,avgdev,best_x,best_z,best_y,rise,best_dist,mass,score};return 1;
}
int main(int argc,char**argv){
 if(argc!=7){fprintf(stderr,"usage: search SEED CENTER_X CENTER_Z RADIUS COARSE_STEP OUTPUT\n");return 2;}
 int64_t ss=strtoll(argv[1],0,10);int cx=atoi(argv[2]),cz=atoi(argv[3]),radius=atoi(argv[4]),step=atoi(argv[5]);const char*outpath=argv[6];
 setupGenerator(&g,MC_26_2_S8,0);applySeed(&g,DIM_OVERWORLD,(uint64_t)ss);initSurfaceNoise(&sn,DIM_OVERWORLD,(uint64_t)ss);
 Region regions[MAX_REGIONS];int rc=0;Result *coarse=malloc(sizeof(Result)*MAX_RESULTS);int cc=0;
 for(int z=cz-radius;z<=cz+radius;z+=step)for(int x=cx-radius;x<=cx+radius;x+=step){Result r;if(analyse(x,z,&r)&&cc<MAX_RESULTS)coarse[cc++]=r;}
 qsort(coarse,cc,sizeof(Result),cmp);
 for(int i=0;i<cc&&rc<MAX_REGIONS;i++){int dup=0;for(int j=0;j<rc;j++){long long dx=coarse[i].x-regions[j].x,dz=coarse[i].z-regions[j].z;if(dx*dx+dz*dz<1024LL*1024LL){dup=1;break;}}if(!dup)regions[rc++]=(Region){coarse[i].x,coarse[i].z};}
 free(coarse);
 Result *all=malloc(sizeof(Result)*MAX_RESULTS);int ac=0;
 for(int q=0;q<rc;q++)for(int dz=-512;dz<=512;dz+=32)for(int dx=-512;dx<=512;dx+=32){Result r;if(analyse(regions[q].x+dx,regions[q].z+dz,&r)&&ac<MAX_RESULTS)all[ac++]=r;}
 qsort(all,ac,sizeof(Result),cmp);FILE*out=fopen(outpath,"w");if(!out)return 3;
 fprintf(out,"rank,x,z,y,plains_pct,relief,avg_deviation,mountain_x,mountain_z,mountain_y,rise,mountain_distance,mountain_mass,score\n");
 Result kept[150];int kc=0;for(int i=0;i<ac&&kc<150;i++){int dup=0;for(int j=0;j<kc;j++){long long dx=all[i].x-kept[j].x,dz=all[i].z-kept[j].z;if(dx*dx+dz*dz<300LL*300LL){dup=1;break;}}if(dup)continue;kept[kc]=all[i];fprintf(out,"%d,%d,%d,%.1f,%.1f,%.1f,%.2f,%d,%d,%.1f,%.1f,%.1f,%.2f,%.2f\n",kc+1,all[i].x,all[i].z,all[i].y,all[i].plains_pct,all[i].relief,all[i].avg_deviation,all[i].mountain_x,all[i].mountain_z,all[i].mountain_y,all[i].rise,all[i].distance,all[i].mass,all[i].score);kc++;}
 fclose(out);free(all);fprintf(stderr,"coarse=%d regions=%d refined=%d kept=%d\n",cc,rc,ac,kc);return 0;
}
