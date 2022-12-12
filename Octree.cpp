#include <iostream>
#include <string>
#include <memory>

class coor_value
{
public:
	coor_value() {
		value = false;
	}

//	~coor_value();

	void set_value(bool fill) {
		value = fill;
		return;
	}

	bool get_value() {
		return value;
	}

private:
	bool value;
};

class xyz_array{
public:
    xyz_array() {}
    
    bool get_coor(int x, int y, int z){
        return xyz[x][y][z].get_value();
    }

    void coor_true(int x, int y, int z){
        xyz[x][y][z].set_value(true);
        return;
    }
    
    void coor_false(int x, int y, int z){
        xyz[x][y][z].set_value(false);
        return;
    }

private:
    coor_value xyz[8][8][8];
};

class oct_node{
public:
    oct_node(){
        for (int i = 0; i < 4; i++){
            octals[i] = NULL;
        }
    }

    std::shared_ptr<oct_node> oct_it(std::shared_ptr<oct_node> above, std::shared_ptr<oct_node> below, xyz_array three_d, int x, int y, int z){
        std::shared_ptr<oct_node> octi (new oct_node);             // build octree 'octi' pointer
        
        if (x == 8){
            return octi;
        }
        
        if (below == NULL && above == NULL){
            octi->rank = 1;
            std::cout << " set rank " << octi->rank << std::endl;
        }
        
        if (above != NULL) {                                  // layer ranks increment by one
            octi->rank = (above->rank) - 1;
            octi->parent = above;
            int i = 0;
            while (above->octals[i] != NULL && i < 4){
                i++;}            
            above->octals[i] = octi;
            std::cout << " set rank " << octi->rank << " at position " << i << std::endl;
        }
        
        if (below != NULL) {                                  // layer ranks increment by one
            int i = 0;
            while (octi->octals[i] != NULL && i < 4){
                i++;}
            octi->rank = (below->rank) + 1;
            octi->octals[i] = below;
            std::cout << " set rank " << octi->rank << " And set below as " << below->x_first << below->y_first << below->z_first << " - " << below->x_last << below->y_last << below->z_last << " at " << i << std::endl;
        }
        
        if(octi->rank > 1){
            int i = 0;
            while (octi->octals[i] != NULL || i == 4){
                i++;}
            if (i < 4){
                std::cout << " Building Down at " << i << std::endl;
                return oct_it(octi, NULL, three_d, x , y, z);}
            else if (i == 4){
                for (int idx = 0; idx < 4; idx++)
                {
                    if(octi->octals[idx]->oct_value == true){
                        octi->oct_value = true;
                        std::cout << "Setting to True" << std::endl;
                    }
                }
                octi->x_first = octi->octals[0]->x_first;
                octi->y_first = octi->octals[0]->y_first;
                octi->z_first = octi->octals[0]->z_first;
                octi->x_last = octi->octals[3]->x_last;
                octi->y_last = octi->octals[3]->y_last;
                octi->z_last = octi->octals[3]->z_last;
                std::cout << " Building Up From " << octi->x_first << octi->y_first << octi->z_first << " - " << octi->x_last << octi->y_last << octi->z_last  << std::endl;
                return oct_it(NULL, octi, three_d, x , y, z);}
        }

        if (octi->rank == 1){                            // specific coor_values only entered in rank zero octree nodes 
            for( int i = 0; i < 4; i++ ){
                octi->octals[i] = sub_set(three_d.get_coor(x,y,z), octi, x, y, z);   // coor_value set with umbrella set
                if (three_d.get_coor(x,y,z) == true)
                {
                    octi->oct_value = true;
                    std::cout << "Setting to True" << std::endl;
                }
                //std::cout << "added " << x << y << z << std::endl;
                z++;                                                     // iterate thrugh z coordinates
            }
            if (z == 8){                 // if end of current z coordinate
                z = 0;                    // reset z coordinate (if not on last x line)
                y++;                      // iterate one y coordinate
                //std::cout << "z set to " << z << std::endl;}
            if (y == 8){                // if end of current y coordinate
                y = 0;                   // reset y coordinate (if not on last x line)
                //std::cout << " y set to " << y << std::endl;
                x++;                     // iterate one x coordinate
                //std::cout << " x set to " << x << std::endl;
            }
            octi->x_first = octi->octals[0]->x_first;
            octi->y_first = octi->octals[0]->y_first;
            octi->z_first = octi->octals[0]->z_first;
            octi->x_last = octi->octals[3]->x_first;
            octi->y_last = octi->octals[3]->y_first;
            octi->z_last = octi->octals[3]->z_first;
            std::cout << " Rank 1 filled with " << octi->x_first << octi->y_first << octi->z_first << " - " << octi->x_last << octi->y_last << octi->z_last  << std::endl;
            }
        }

        if(x != 8){            // recursion loop ends if reached the end of all coordinates
            int i = 0;
            while (find_open(octi)->octals[i] != NULL && i < 4){
                i++;
            }
            if (i < 4){
                return oct_it(find_open(octi), NULL, three_d, x, y, z);
            }
            else if (i == 4){
                return oct_it(NULL, octi, three_d, x, y, z);
            }
        }
        else if(x == 8){
            return octi;
        }
    }

    std::shared_ptr<oct_node> sub_set(bool value, std::shared_ptr<oct_node> above, int x, int y, int z){
        std::shared_ptr<oct_node> under (new oct_node);
        under->oct_value = value;
        under->rank = 0;
        under->parent = above;
        under->x_first = x;
        under->y_first = y;
        under->z_first = z;
        return under;
    }
    
    std::shared_ptr<oct_node> find_open (std::shared_ptr<oct_node> last){
        int i = 0;
        while (last->octals[i] != NULL && i < 4){
            i++;
        }
        if(i < 4){
            return last;
        }
        else if (i = 4){
            if (last->parent == NULL){
                return last;
            }
            else if (last->parent != NULL){
                return find_open(last->parent);
            }
        }
    }

private:
    int rank;
    std::shared_ptr<oct_node> octals [4];
    bool oct_value = false;
    std::shared_ptr<oct_node> parent = NULL;
    int x_first = -1;
    int y_first = -1;
    int z_first = -1;
    int x_last = -1;
    int y_last = -1;
    int z_last = -1;
};

// typedef std::shared_ptr<coor_value> xyz[16][16][16];

int main(){
    xyz_array three_d;
    std::shared_ptr<oct_node> oct_top (new oct_node);
    
	three_d.coor_true(5,6,7);
	std::cout << " after set value: " << three_d.get_coor(5,6,7) << std::endl;
    
    oct_top = oct_top->oct_it(NULL, NULL, three_d, 0, 0, 0);

	return 0;
}