#ifndef __ENKF_TYPES_H__
#define __ENKF_TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <arg_pack.h>
typedef double (transform_ftype) (double , const arg_pack_type *);



/*
  Observe that seemingly random numbers are used in these enum definitions, 
  that is to be able to catch it if a wrong constant is used.
*/


/*
  Observe that these are used as bitmask's, i.e. they must be a power of 2 series.
*/



typedef enum {invalid          =  0  , /**/
              parameter        =  1  , /* A parameter which is updated with enkf                                                  */
	      dynamic          =  2  , /* Dynamic data */
	      ecl_static       =  4  , /* Keywords like XCON++ from eclipse restart files - which are just dragged along          */ 
	      all_types        =  7 }  enkf_var_type;






/*
  The static can be confusing - because the impl_type STATIC and the
  enkf_var_type ecl_static symbolize perfectly the same thing.
*/

/* 
   For instance the pressure is implemented with a field, and behaves
   as a ecl_restart variable. The permeability is also implemented as
   a field, but this is a parameter.
*/

/*
  These correspond to implementation types. The numbers are on disk,
  and should **NOT BE UPDATED**.
*/
typedef enum {INVALID 	   = 0   , 
	      STATIC  	   = 100 ,
	      MULTZ   	   = 101 ,
	      MULTFLT 	   = 102 ,      
	      EQUIL   	   = 103 ,         
	      FIELD   	   = 104 ,       /* WELL has been removed */
	      GEN_KW  	   = 107 ,        
	      RELPERM 	   = 108 ,       
	      HAVANA_FAULT = 109 ,   
	      SUMMARY      = 110 ,      
              TPGZONE      = 111 ,       
              GEN_DATA     = 113 , /* Mind the gap - a type has been removed. */
	      PILOT_POINT  = 114 } enkf_impl_type;

/* 
   Should update the functions enkf_types_get_impl_name() and
   enkf_types_get_impl_type__() when this enum is updated.
   In addition to enkf_config_add_type().
*/






/*
  These types are logical types, describing how the parameter behaves in the EnKF
  loop.
*/


typedef enum {active_off = 200 , active_on , active_at , active_after , active_before} enkf_active_type;
typedef enum {abs_error   = 0  , rel_error = 1 , rel_min_abs_error = 2} enkf_obs_error_type; /* Should not have enkf_ prefix */


/*
  typedef enum {WELL_OBS = 0 , POINT_OBS = 1} enkf_obs_type;
*/

typedef enum {nolog = 0 , log_input_mask = 1 , log_enkf_mask = 2 , log_output_mask = 4 , log_all = 7} enkf_logmode_enum;


typedef enum   {undefined   = 0 , 
		serialized  = 1,
		forecast    = 2, 
		analyzed    = 4,
		both        = 6} state_enum;  /* It is important that both == (forecast + analyzed) */
/**
   The state == both is used for output purposes (getting both forecast and analyzed).
*/




/** 
    These are 2^n bitmasks - truncate_minmax == truncate_min + truncate_max. 
*/
typedef enum {truncate_none   = 0,
	      truncate_min    = 1,
	      truncate_max    = 2,
	      truncate_minmax = 3} truncation_type;




/**
   This enum is used to differentiate between different types of
   run. The point is that depending on this mode we can be more or
   less restrictive on the amount of input we require from the user. 

   In mode enkf_assimlation ( which is the default ), we require quite
   a lot of info, whereas in the case screening_experiment we require
   a lot less.

   screening_experiment: 
      - SIZE
      - RUNPATH
      - ECLBASE
      - START_TIME
      - SCHEDULE_FILE
      - DATA_FILE
      - FORWARD_MODEL.

   ensemble_experiment:
      - ENSPATH
      - INIT_FILE (or estimation of EQUIL)

   enkf_assmilation:
      - RESULT_PATH

*/

typedef enum { enkf_assimilation    = 1, 
	       ensemble_experiment  = 2,
	       screening_experiment = 3} run_mode_type;


typedef enum { lock_lockf = 1,
	       lock_file  = 2,
	       lock_none  = 3 } lock_mode_type;


/*****************************************************************/
/*
  integer, intent(in) :: mode 
  ! first integer means (EnKF=1, SQRT=2)
  ! Second integer is pseudo inversion
  !  1=eigen value pseudo inversion of SS'+(N-1)R
  !  2=SVD subspace pseudo inversion of SS'+(N-1)R
  !  3=SVD subspace pseudo inversion of SS'+EE'
*/


typedef enum {
  enkf_standard = 10,
  enkf_sqrt     = 20
} enkf_mode_type ;
	       

typedef enum {
  eigen_SS_N1_R = 1,
  SVD_SS_N1_R   = 2,
  SVD_SS_EE     = 3
} pseudo_inversion_type ;

/*****************************************************************/

/**
   This enum is used when we are setting up the dependencies between
   observations and variables. The modes all_active and inactive are
   sufficient information, for the values partly active we need
   additional information.

   The same type is used both for variables (PRESSURE/PORO/MULTZ/...)
   and for observations.
*/

typedef enum {
  all_active 	= 1,       /* The variable/observation is fully active, i.e. all cells/all faults/all .. */
  inactive   	= 2,       /* Fully inactive */
  partly_active = 3        /* Partly active - must supply additonal type spesific information on what is active.*/
} active_mode_type; 


  



enkf_impl_type    enkf_types_get_impl_type(const char * );
const char      * enkf_types_get_impl_name(enkf_impl_type );
enkf_impl_type    enkf_types_check_impl_type(const char * );
#ifdef __cplusplus
}
#endif
#endif
