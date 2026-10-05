#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "generator.h"
#include "biomes.h"

#define MAX_RESULTS 50000

typedef struct {
    int x, z;
    float y, plains_pct, relief, avg_deviation;
    int mountain_x, mountain_z;
    float mountain_y, rise, distance, mass, edge_distance, edge_rise, score;
} Result;

static Generator g;
static SurfaceNoise sn;

static void terrain(int x, int z, float *y, int *id) {
    mapApproxHeight(y, id, &g, &sn, x >> 2, z >> 2, 1, 1);
}
static int is_plains(int id) { return id == plains || id == sunflower_plains; }
static int cmp(const void *a,const void *b) {
    float x=((const Result*)a)->score, y=((const Result*)b)->score;
    return x<y ? 1 : x>y ? -1 : 0;
}
static int analyse(int x,int z,Result *r) {
    float cy; int cid; terrain(x,z,&cy,&cid); if(!is_plains(cid)) return 0;
    int plains=0,total=0; float miny=9999,maxy=-9999,sum=0;
    for(int dz=-112;dz<=112;dz+=28) for(int dx=-112;dx<=112;dx+=28) {
        float y; int id; terrain(x+dx,z+dz,&y,&id); total++; if(is_plains(id)) plains++;
        if(y<miny) miny=y; if(y>maxy) maxy=y; sum+=y;
    }
    float avg=sum/total, plains_pct=100.0f*plains/total, relief=maxy-miny, devsum=0;
    for(int dz=-112;dz<=112;dz+=28) for(int dx=-112;dx<=112;dx+=28) {
        float y; int id; terrain(x+dx,z+dz,&y,&id); devsum+=fabsf(y-avg);
    }
    float avgdev=devsum/total; if(plains_pct<50.0f) return 0;
    float best_score=-99999,best_y=-9999,best_dist=9999; int best_x=0,best_z=0,high=0,mtot=0;
    for(int dz=-800;dz<=800;dz+=32) for(int dx=-800;dx<=800;dx+=32) {
        float dist=sqrtf((float)(dx*dx+dz*dz)); if(dist<160||dist>800) continue;
        float y; int id; terrain(x+dx,z+dz,&y,&id); mtot++; float rise=y-avg;
        if(rise>=40) high++; float target=rise-(dist*0.035f);
        if(target>best_score){best_score=target;best_y=y;best_dist=dist;best_x=x+dx;best_z=z+dz;}
    }
    float rise=best_y-avg,mass=mtot?100.0f*high/mtot:0; if(rise<45) return 0;
    /* Required Everhome geometry: the mountain must begin directly beside the settlement plains.
       Measure from the edge of the 224x224 settlement sample toward the selected mountain. */
    float vx=(float)(best_x-x), vz=(float)(best_z-z), vlen=sqrtf(vx*vx+vz*vz);
    float edge_dist=9999.0f, edge_rise=-9999.0f;
    if(vlen>0){
        float ux=vx/vlen, uz=vz/vlen;
        for(int d=112;d<=320;d+=16){
            int sx=x+(int)lroundf(ux*d), sz=z+(int)lroundf(uz*d); float sy; int sid; terrain(sx,sz,&sy,&sid);
            float srise=sy-avg;
            if(!is_plains(sid) && srise>=20.0f){edge_dist=(float)(d-112); edge_rise=srise; break;}
        }
    }
    /* Mountain foot must start within 96 blocks of the settlement sample edge. */
    if(edge_dist>96.0f) return 0;
    float flat=15.0f-avgdev; if(flat<0) flat=0; float prox=800.0f-best_dist; if(prox<0) prox=0;
    float adjacency=96.0f-edge_dist; if(adjacency<0) adjacency=0;
    float score=plains_pct*0.40f+flat*2.0f+rise*0.30f+mass*0.80f+prox*0.025f+adjacency*0.10f;
    *r=(Result){x,z,avg,plains_pct,relief,avgdev,best_x,best_z,best_y,rise,best_dist,mass,edge_dist,edge_rise,score};
    return 1;
}
int main(int argc,char **argv) {
    if(argc!=7){fprintf(stderr,"usage: search SEED CENTER_X CENTER_Z RADIUS COARSE_STEP OUTPUT\n");return 2;}
    int64_t signed_seed=strtoll(argv[1],NULL,10); uint64_t seed=(uint64_t)signed_seed;
    int cx=atoi(argv[2]),cz=atoi(argv[3]),radius=atoi(argv[4]),step=atoi(argv[5]); const char *outpath=argv[6];
    if(radius<0||step<32){fprintf(stderr,"invalid radius/step\n");return 2;}
    setupGenerator(&g,MC_26_2_S8,0); applySeed(&g,DIM_OVERWORLD,seed); initSurfaceNoise(&sn,DIM_OVERWORLD,seed);
    Result *results=malloc(sizeof(Result)*MAX_RESULTS); if(!results) return 3; int n=0;
    for(int z=cz-radius;z<=cz+radius;z+=step) for(int x=cx-radius;x<=cx+radius;x+=step) {
        Result coarse;
        if(!analyse(x,z,&coarse)) continue;
        for(int dz=-512;dz<=512;dz+=32) for(int dx=-512;dx<=512;dx+=32) {
            Result r; if(analyse(x+dx,z+dz,&r)&&n<MAX_RESULTS) results[n++]=r;
        }
    }
    qsort(results,n,sizeof(Result),cmp); FILE *out=fopen(outpath,"w"); if(!out){free(results);return 4;}
    fprintf(out,"rank,x,z,y,plains_pct,relief,avg_deviation,mountain_x,mountain_z,mountain_y,rise,mountain_distance,mountain_mass,mountain_edge_distance,mountain_edge_rise,score\n");
    Result kept[150]; int k=0;
    for(int i=0;i<n&&k<150;i++){
        int dup=0; for(int j=0;j<k;j++){long long dx=results[i].x-kept[j].x,dz=results[i].z-kept[j].z;if(dx*dx+dz*dz<90000LL){dup=1;break;}}
        if(dup) continue; kept[k]=results[i];
        fprintf(out,"%d,%d,%d,%.1f,%.1f,%.1f,%.2f,%d,%d,%.1f,%.1f,%.1f,%.2f,%.1f,%.1f,%.2f\n",
          k+1,results[i].x,results[i].z,results[i].y,results[i].plains_pct,results[i].relief,results[i].avg_deviation,
          results[i].mountain_x,results[i].mountain_z,results[i].mountain_y,results[i].rise,results[i].distance,results[i].mass,results[i].edge_distance,results[i].edge_rise,results[i].score); k++;
    }
    fclose(out); free(results); fprintf(stderr,"kept %d candidates\n",k); return 0;
}
