/* DESCRIPTION:
 * First include the libraries.
 * Just to be more realistic i decided to create some functions like: open_door(), provide_drinks()..., which they basicly use build-in sleep() functions just to keep bussy.
 * After that we create world_rank and world_size, to determine the rank and the size of the processes in MPI_COMM_WORLD.
 * From the original group 'orig_group' we create two sub groups called 'new_group', each of them are devided ranks1(from 0 to 4, guys + pizza party guy) and ranks2(5 to 9 ladies).
 * Since there are 10 people, we devide the rooms based on the nuber of the people, people/2 < people -> guys(plus pizza party guy) and people/2 > people -> ladies
 * There are 20 salami pieces, based on the group, guys and ladies exchange salami pieces, in a simmilar logic as ping_pong example
 * After than if the rank is 0, wich means if it is the pizza party guy, then do some more work, like: open_door()... 
 * Finally MPI_Barrier(), whait for everyone to be finnished and then go to the party() togather.
*/
#include "mpi.h" 
#include <stdio.h>
#define PEOPLE 10 // 9 friends + party guys

// Party guy has alot to do...
int time;
int open_door(time) { sleep(time); }
int provide_drinks(time) { sleep(time); }
int eat_alot(time) { sleep(time); }
int talk_alot(time) { sleep(time); }

int party() {printf("we\'re done with the pizza now party time, yuhuuu \n");}

int main(argc,argv) 

int argc; 
char *argv[]; { 
int world_rank, world_size, new_rank, 
  ranks1[5]={0,1,2,3,4}, ranks2[5]={5,6,7,8,9}; 

MPI_Group orig_group, new_group; 
MPI_Comm new_comm; 
MPI_Init(&argc,&argv); 
 MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // World rank 
 MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Number of precesses 

/* Extract the original group handle */ 
MPI_Comm_group(MPI_COMM_WORLD, &orig_group); 
      
/* Divide tasks into two distinct groups based upon rank */ 
// Seperate into 2 rooms, guys and ladies
if (world_rank < PEOPLE/2) { MPI_Group_incl(orig_group, PEOPLE/2, ranks1, &new_group);} 
else { MPI_Group_incl(orig_group, PEOPLE/2, ranks2, &new_group); } 

/* Create new communicator and then perform collective communications */ 
// Collective communication between guys and ladies
MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

// MPI_Allreduce(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, new_comm); 

MPI_Group_rank(new_group, &new_rank); 

 int SALAMI_PIECES = 20;
 int salami_pieces_count = 0;
 while(salami_pieces_count < SALAMI_PIECES) { 
   // Guys
   if(new_rank < PEOPLE/2) { 
     salami_pieces_count++;
     MPI_Send(&salami_pieces_count, 1, MPI_INT, new_rank, 0, new_comm); 
     /* printf("%d sent Salami Picees  %d to %d\n",
	world_rank, salami_pieces_count, new_rank); */
     printf("exchange some salamies : %d \n", salami_pieces_count);
   } else {
     // Ladies
       talk_alot(10);
       MPI_Recv(&salami_pieces_count, 1, MPI_INT, new_rank, 0, new_comm, MPI_STATUS_IGNORE);
       /* printf("%d received Salami Pieces %d from %d\n",
	  world_rank, salami_pieces_count, new_rank); */
   }
 }


if(world_rank == 0 || new_rank == 0) { 
  open_door(3);
   provide_drinks(10);
  eat_alot(5);
  talk_alot(5);
 }

// Wait for everyone and then go to a party
MPI_Barrier(MPI_COMM_WORLD);
MPI_Barrier(new_comm);
party();

//printf("rank= %d newrank= %d recvbuf= %d\n",world_rank,new_rank,salami_pieces_count); 

MPI_Finalize(); 

} 
