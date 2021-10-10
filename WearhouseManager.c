#include <stdio.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination){
	Package *pachet = malloc(sizeof(Package));
		if(pachet == NULL) return NULL;	
	pachet->destination = malloc(30 * sizeof(char));
		if(pachet->destination == NULL){
 			free(pachet);
  			return NULL;}
	pachet->priority = priority;
	if(destination == NULL)
		pachet->destination = NULL;
	else
		strcpy(pachet->destination, destination);
	return pachet;
}
	// TODO 3.1

void destroy_package(Package* package){
	if(package == NULL) return;
	package->priority = 0;
	if(package->destination != NULL)
		free(package->destination);
	free(package);
	//TODO: 3.1
}

Manifest* create_manifest_node(void){
	struct Manifest *manifest = malloc(sizeof(Manifest));
	if(manifest == NULL) return NULL;
	manifest -> next = NULL;
	manifest -> prev = NULL;
	manifest -> package = NULL;
	return manifest;
}
void destroy_manifest_node(Manifest* manifest_node){
	if(manifest_node == NULL) return;
	destroy_package(manifest_node->package);
	free(manifest_node);
	// TODO: 3.1.

}

Wearhouse* create_wearhouse(long capacity){
	Wearhouse *w = (Wearhouse*)malloc(sizeof(Wearhouse));
	if(capacity == 0){
			w->packages = NULL;
		return NULL;}
	if(w == NULL) return NULL;
	w->capacity = capacity;
	w->size = 0;
	if(capacity != 0){
		w->packages = (Package**)malloc(capacity*sizeof(Package*));
		for(long i = 0; i < capacity; i++){
			w->packages[i] = (Package*)malloc(sizeof(Package));
				w->packages[i] = NULL;}
			return w;
		}
}

Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	if(w->size == 0) return 1;
	else
		return 0;
	// TODO: 3.2
}

long wearhouse_is_full(Wearhouse *w){
	// TODO: 3.2
	if(w->size == w->capacity) return 1;
	else 
	return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	// TODO: 3.2
	long numar = 0;
	for(long i = 0; i < w->size;i++){
		if(numar < w->packages[i]->priority)
			numar = w->packages[i]->priority;
	}
	return numar;
}

long wearhouse_min_package_priority(Wearhouse *w){
	// TODO: 3.2
	long numar = 0;
	for(long i = 0; i < w->size;i++){
		if(numar > w->packages[i]->priority)
			numar = w->packages[i]->priority;
	}
	return numar;
}

void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){
	if(wearhouse == NULL) return ;
	for(long i = 0; i<wearhouse->size;i++){
		destroy_package(wearhouse->packages[i]);
	}
	free(wearhouse->packages);
	free(wearhouse);
	// TODO: 3.2
}


Robot* create_robot(long capacity){
	struct Robot* r = malloc(sizeof(Robot));
	if(r == NULL) return NULL;
	r->manifest = NULL;
	r->size = 0;
	r->capacity = capacity;
	r->next = NULL;
	// TODO: 3.2
	//return NULL;
}

int robot_is_full(Robot* robot){
	if(robot->size == robot->capacity) return 1;
	else
	// TODO: 3.2
	return 0;
}

int robot_is_empty(Robot* robot){
	// TODO: 3.2
	if(robot->size == 0) return 1;
	else
	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	// TODO: 3.2
	Package* sal = NULL;
	for(long i = 0 ; i < w->size ; i++){
		if(w->packages[i]->priority == priority){
			sal = w->packages[i];
		break;
			}
	}
	return sal;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	// TODO: 3.2
	if(package == NULL) return;
	for(long i = 0; i <= w->size; i++){
		if(w->packages[i] == package){
				for(long k=i; k < w->size-1;k++)
					w->packages[k] = w->packages[k+1];
			break;
			}
	//}
}
			w->size-- ;	
}

void robot_load_one_package(Robot* robot, Package* package){
	// TOD2O:  3.2
	Manifest* M = (Manifest*) malloc(sizeof(Manifest));
	M->package = package;
	M->prev = M->next = NULL;

	if (robot_is_empty(robot)) {
		robot->manifest = M;
		robot->size++;
		return;
	}
	if (robot->manifest->package->priority == M->package->priority) {
		if (strcmp(robot->manifest->package->destination, M->package->destination) > 0) {

			M->next = robot->manifest;
			M->next->prev = M;
			robot->manifest = M;
			robot->size++;
			return;
		}	
		else if (strcmp(robot->manifest->package->destination, M->package->destination) < 0) {

			M->prev = robot->manifest;
			M->prev->next = M;
			robot->manifest = M;
			robot->size++;
			return;
		}
		else {
			Manifest* i = robot->manifest;
			while((i->next != NULL) && (strcmp(robot->manifest->package->destination, i->package->destination) > 0))
				i = i->next;
			M->next = i->next;
			if (i->next != NULL)
				M->next->prev = i;
			i->next = M;
			M->prev =  i;
			robot->size++;
			return;
		}
	}
	if (robot->manifest->package->priority < M->package->priority) {
			M->next = robot->manifest;
			M->next->prev = M;
			robot->manifest = M;
			robot->size++;
			return;
		}
		else {
			Manifest* i = robot->manifest;
			while((i->next != NULL) && (robot->manifest->package->priority > i->package->priority))
				i = i->next;
			M->next = i->next;
			if (i->next != NULL)
				M->next->prev = i;
			i->next = M;
			M->prev =  i;
			robot->size++;
			return;
		}	
	
}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	// TODO: 3.2

	return 0;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	// TODO: 3.2
	return NULL;
}

void destroy_robot(Robot* robot){
	// TODO: 3.2
	Manifest* aux;
	while(robot->manifest != NULL){

	}
	free(robot);

}

void robot_unload_packages(Truck* truck, Robot* robot){
	// TODO: 3.3
}



// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){
	// TODO: 3.3
	struct Truck* t = malloc(sizeof(Truck));
	if(t == NULL) return NULL;
	t->destination = malloc(30 * sizeof(char));
	//if(t->destination == NULL){
	//	free(t);
	//	return NULL;
	//}
	if(destination == NULL)
		t->destination = NULL;
	else
		strcpy(t->destination, destination);
	t->manifest = NULL;
	t->unloading_robots = NULL;
	t->size = 0;
	t->capacity = capacity;
	t->in_transit_time = 0;
	t->transit_end_time = transit_time;
	t->departure_time = departure_time;
	t->next = NULL;
	return t;
}

int truck_is_full(Truck *truck){
	if(truck->size == truck->capacity) return 1;
	else
		return 0;
	// TODO: 3.3

	return 0;
}

int truck_is_empty(Truck *truck){
	if(truck->size == 0) return 1;
	else 
		return 0;
	// TODO: 3.3
}

long truck_destination_robots_unloading_size(Truck* truck){

	// TODO: 3.3
	return 0;
}


void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck){
	// TODO: 3.3
	free(truck->manifest);
	free(truck->unloading_robots);

	free(truck);

}

Parkinglot* create_parkinglot(void){
	// TODO: 3.4
	 Parkinglot* p =(Parkinglot*)malloc(sizeof(Parkinglot));
	p->arrived_trucks = (Truck*)malloc(sizeof(Truck));
	p->departed_trucks = (Truck*)malloc(sizeof(Truck));
	p->pending_robots = (Robot*)malloc(sizeof(Robot));
	p->standby_robots = (Robot*)malloc(sizeof(Robot));
	p->arrived_trucks->next = p->arrived_trucks;
	p->departed_trucks->next = p->departed_trucks; 
	p->pending_robots->next = p->pending_robots;
	p->standby_robots->next = p->standby_robots;
	return p;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	// TODO: 3.4
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){
	// TODO: 3.4
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->pending_robots->next == parkinglot->pending_robots)
		return 0;
	else 
		return 1;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	// TODO: 3.4
	//for(long i = 0;i < parkinglot->arrived_trucks->capacity ; i++){
		if(parkinglot->arrived_trucks->size != 0)
		return 1;
	else return 0;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->departed_trucks->next != parkinglot->departed_trucks)
		return 1;
	else 
		return 0;
}


void destroy_parkinglot(Parkinglot* parkinglot){
	destroy_truck(parkinglot->arrived_trucks);
	destroy_truck(parkinglot->departed_trucks);
	destroy_robot(parkinglot->pending_robots);
	destroy_robot(parkinglot->standby_robots);
	free(parkinglot);

}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}


void truck_departed(Parkinglot *parkinglot, Truck* truck){
	// TODO: 3.5
	// Search through arrived list, if exists node is found remove it
	// Note: this must remove the node from the list, NOT deallocate it
}


void truck_arrived(Parkinglot *parkinglot, Truck* truck){
	if(parkinglot == NULL || truck == NULL) return;

	// TODO: 3.5
	// Search through departed list, if exists node is found remove it
	// Note: this must remove the node not deallocate it

}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	// TODO:  3.5

}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	// TODO: 3.5
	Truck* t_it = parkinglot->arrived_trucks->next;
	while(t_it != parkinglot->arrived_trucks){
		Truck* aux = t_it;
		t_it = t_it->next;
		if(day_hour == aux->transit_end_time){
			truck_transfer_unloading_robots(parkinglot, aux);
			truck_departed(parkinglot, aux);
		}
	}

}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	// TODO: 3.5
	Truck* t_it = parkinglot->departed_trucks->next;
	while(t_it != parkinglot->departed_trucks){
		Truck *aux = t_it;
		t_it = t_it->next;	
		if(aux->in_transit_time = aux->transit_end_time){
			aux->in_transit_time = 0;
			truck_arrived(parkinglot, aux);
		}
	}
}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

