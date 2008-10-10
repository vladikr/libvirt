/* -*- c -*-
 * libvirt.h:
 * Summary: core interfaces for the libvirt library
 * Description: Provides the interfaces of the libvirt library to handle
 *              virtualized domains
 *
 * Copy:  Copyright (C) 2005,2006 Red Hat, Inc.
 *
 * See COPYING.LIB for the License of this software
 *
 * Author: Daniel Veillard <veillard@redhat.com>
 */

#ifndef __VIR_VIRLIB_H__
#define __VIR_VIRLIB_H__

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VIR_DEPRECATED
  /* The feature is present in gcc-3.1 and newer.  */
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#  define VIR_DEPRECATED __attribute__((__deprecated__))
# else
#  define VIR_DEPRECATED /* nothing */
# endif
#endif /* VIR_DEPRECATED */

/**
 * virConnect:
 *
 * a virConnect is a private structure representing a connection to
 * the Hypervisor.
 */
typedef struct _virConnect virConnect;

/**
 * virConnectPtr:
 *
 * a virConnectPtr is pointer to a virConnect private structure, this is the
 * type used to reference a connection to the Hypervisor in the API.
 */
typedef virConnect *virConnectPtr;

/**
 * virDomain:
 *
 * a virDomain is a private structure representing a domain.
 */
typedef struct _virDomain virDomain;

/**
 * virDomainPtr:
 *
 * a virDomainPtr is pointer to a virDomain private structure, this is the
 * type used to reference a domain in the API.
 */
typedef virDomain *virDomainPtr;

/**
 * virDomainState:
 *
 * A domain may be in different states at a given point in time
 */
typedef enum {
     VIR_DOMAIN_NOSTATE = 0, /* no state */
     VIR_DOMAIN_RUNNING = 1, /* the domain is running */
     VIR_DOMAIN_BLOCKED = 2, /* the domain is blocked on resource */
     VIR_DOMAIN_PAUSED  = 3, /* the domain is paused by user */
     VIR_DOMAIN_SHUTDOWN= 4, /* the domain is being shut down */
     VIR_DOMAIN_SHUTOFF = 5, /* the domain is shut off */
     VIR_DOMAIN_CRASHED = 6  /* the domain is crashed */
} virDomainState;

/**
 * virDomainInfoPtr:
 *
 * a virDomainInfo is a structure filled by virDomainGetInfo() and extracting
 * runtime information for a given active Domain
 */

typedef struct _virDomainInfo virDomainInfo;

struct _virDomainInfo {
    unsigned char state;        /* the running state, one of virDomainState */
    unsigned long maxMem;       /* the maximum memory in KBytes allowed */
    unsigned long memory;       /* the memory in KBytes used by the domain */
    unsigned short nrVirtCpu;   /* the number of virtual CPUs for the domain */
    unsigned long long cpuTime; /* the CPU time used in nanoseconds */
};

/**
 * virDomainInfoPtr:
 *
 * a virDomainInfoPtr is a pointer to a virDomainInfo structure.
 */

typedef virDomainInfo *virDomainInfoPtr;

/**
 * virDomainCreateFlags:
 *
 * Flags OR'ed together to provide specific behaviour when creating a
 * Domain.
 */
typedef enum {
     VIR_DOMAIN_NONE = 0
} virDomainCreateFlags;

/**
 * virNodeInfoPtr:
 *
 * a virNodeInfo is a structure filled by virNodeGetInfo() and providing
 * the information for the Node.
 */

typedef struct _virNodeInfo virNodeInfo;

struct _virNodeInfo {
    char model[32];     /* string indicating the CPU model */
    unsigned long memory;/* memory size in kilobytes */
    unsigned int cpus;  /* the number of active CPUs */
    unsigned int mhz;   /* expected CPU frequency */
    unsigned int nodes; /* the number of NUMA cell, 1 for uniform mem access */
    unsigned int sockets;/* number of CPU socket per node */
    unsigned int cores; /* number of core per socket */
    unsigned int threads;/* number of threads per core */
};


/**
 * virDomainSchedParameterType:
 *
 * A scheduler parameter field type
 */
typedef enum {
    VIR_DOMAIN_SCHED_FIELD_INT     = 1, /* integer case */
    VIR_DOMAIN_SCHED_FIELD_UINT    = 2, /* unsigned integer case */
    VIR_DOMAIN_SCHED_FIELD_LLONG   = 3, /* long long case */
    VIR_DOMAIN_SCHED_FIELD_ULLONG  = 4, /* unsigned long long case */
    VIR_DOMAIN_SCHED_FIELD_DOUBLE  = 5, /* double case */
    VIR_DOMAIN_SCHED_FIELD_BOOLEAN = 6  /* boolean(character) case */
} virSchedParameterType;

/**
 * VIR_DOMAIN_SCHED_FIELD_LENGTH:
 *
 * Macro providing the field length of virSchedParameter
 */

#define VIR_DOMAIN_SCHED_FIELD_LENGTH 80

/**
 * virDomainSchedParameter:
 *
 * a virDomainSchedParameter is the set of scheduler parameters
 */

typedef struct _virSchedParameter virSchedParameter;

struct _virSchedParameter {
    char field[VIR_DOMAIN_SCHED_FIELD_LENGTH];  /* parameter name */
    int type;   /* parameter type */
    union {
        int i;                          /* data for integer case */
        unsigned int ui;        /* data for unsigned integer case */
        long long int l;        /* data for long long integer case */
        unsigned long long int ul;      /* data for unsigned long long integer case */
        double d;       /* data for double case */
        char b;         /* data for char case */
    } value; /* parameter value */
};

/**
 * virSchedParameterPtr:
 *
 * a virSchedParameterPtr is a pointer to a virSchedParameter structure.
 */

typedef virSchedParameter *virSchedParameterPtr;

/*
 * Fetch scheduler parameters, caller allocates 'params' field of size 'nparams'
 */
int     virDomainGetSchedulerParameters (virDomainPtr domain,
                                         virSchedParameterPtr params,
                                         int *nparams);

/*
 * Change scheduler parameters
 */
int     virDomainSetSchedulerParameters (virDomainPtr domain,
                                         virSchedParameterPtr params,
                                         int nparams);

/**
 * virDomainBlockStats:
 *
 * Block device stats for virDomainBlockStats.
 *
 * Hypervisors may return a field set to ((long long)-1) which indicates
 * that the hypervisor does not support that statistic.
 *
 * NB. Here 'long long' means 64 bit integer.
 */
typedef struct _virDomainBlockStats virDomainBlockStatsStruct;

struct _virDomainBlockStats {
  long long rd_req; /* number of read requests */
  long long rd_bytes; /* number of read bytes */
  long long wr_req; /* number of write requests */
  long long wr_bytes; /* number of written bytes */
  long long errs;   /* In Xen this returns the mysterious 'oo_req'. */
};

/**
 * virDomainBlockStatsPtr:
 *
 * A pointer to a virDomainBlockStats structure
 */
typedef virDomainBlockStatsStruct *virDomainBlockStatsPtr;

/**
 * virDomainInterfaceStats:
 *
 * Network interface stats for virDomainInterfaceStats.
 *
 * Hypervisors may return a field set to ((long long)-1) which indicates
 * that the hypervisor does not support that statistic.
 *
 * NB. Here 'long long' means 64 bit integer.
 */
typedef struct _virDomainInterfaceStats virDomainInterfaceStatsStruct;

struct _virDomainInterfaceStats {
  long long rx_bytes;
  long long rx_packets;
  long long rx_errs;
  long long rx_drop;
  long long tx_bytes;
  long long tx_packets;
  long long tx_errs;
  long long tx_drop;
};

/**
 * virDomainInterfaceStatsPtr:
 *
 * A pointer to a virDomainInterfaceStats structure
 */
typedef virDomainInterfaceStatsStruct *virDomainInterfaceStatsPtr;


/* Domain migration flags. */
typedef enum {
  VIR_MIGRATE_LIVE              = 1, /* live migration */
} virDomainMigrateFlags;

/* Domain migration. */
virDomainPtr virDomainMigrate (virDomainPtr domain, virConnectPtr dconn,
                               unsigned long flags, const char *dname,
                               const char *uri, unsigned long bandwidth);

/**
 * VIR_NODEINFO_MAXCPUS:
 * @nodeinfo: virNodeInfo instance
 *
 * This macro is to calculate the total number of CPUs supported
 * but not necessary active in the host.
 */


#define VIR_NODEINFO_MAXCPUS(nodeinfo) ((nodeinfo).nodes*(nodeinfo).sockets*(nodeinfo).cores*(nodeinfo).threads)

/**
 * virNodeInfoPtr:
 *
 * a virNodeInfoPtr is a pointer to a virNodeInfo structure.
 */

typedef virNodeInfo *virNodeInfoPtr;

/**
 * virConnectFlags
 *
 * Flags when opening a connection to a hypervisor
 */
typedef enum {
    VIR_CONNECT_RO = 1,    /* A readonly connection */
} virConnectFlags;


typedef enum {
    VIR_CRED_USERNAME = 1,     /* Identity to act as */
    VIR_CRED_AUTHNAME = 2,     /* Identify to authorize as */
    VIR_CRED_LANGUAGE = 3,     /* RFC 1766 languages, comma separated */
    VIR_CRED_CNONCE = 4,       /* client supplies a nonce */
    VIR_CRED_PASSPHRASE = 5,   /* Passphrase secret */
    VIR_CRED_ECHOPROMPT = 6,   /* Challenge response */
    VIR_CRED_NOECHOPROMPT = 7, /* Challenge response */
    VIR_CRED_REALM = 8,        /* Authentication realm */
    VIR_CRED_EXTERNAL = 9,     /* Externally managed credential */

    /* More may be added - expect the unexpected */
} virConnectCredentialType;

struct _virConnectCredential {
    int type; /* One of virConnectCredentialType constants */
    const char *prompt; /* Prompt to show to user */
    const char *challenge; /* Additional challenge to show */
    const char *defresult; /* Optional default result */
    char *result; /* Result to be filled with user response (or defresult) */
    unsigned int resultlen; /* Length of the result */
};

typedef struct _virConnectCredential virConnectCredential;
typedef virConnectCredential *virConnectCredentialPtr;


/**
 * virConnectCredCallbackPtr
 *
 * @param authtype type of authentication being performed
 * @param cred list of virConnectCredential object to fetch from user
 * @param ncred size of cred list
 * @param cbdata opaque data passed to virConnectOpenAuth
 *
 * When authentication requires one or more interactions, this callback
 * is invoked. For each interaction supplied, data must be gathered
 * from the user and filled in to the 'result' and 'resultlen' fields.
 * If an interaction can not be filled, fill in NULL and 0.
 *
 * Return 0 if all interactions were filled, or -1 upon error
 */
typedef int (*virConnectAuthCallbackPtr)(virConnectCredentialPtr cred,
                                         unsigned int ncred,
                                         void *cbdata);

struct _virConnectAuth {
    int *credtype; /* List of supported virConnectCredentialType values */
    unsigned int ncredtype;

    virConnectAuthCallbackPtr cb; /* Callback used to collect credentials */
    void *cbdata;
};


typedef struct _virConnectAuth virConnectAuth;
typedef virConnectAuth *virConnectAuthPtr;

extern virConnectAuthPtr virConnectAuthPtrDefault;

/**
 * VIR_UUID_BUFLEN:
 *
 * This macro provides the length of the buffer required
 * for virDomainGetUUID()
 */

#define VIR_UUID_BUFLEN (16)

/**
 * VIR_UUID_STRING_BUFLEN:
 *
 * This macro provides the length of the buffer required
 * for virDomainGetUUIDString()
 */

#define VIR_UUID_STRING_BUFLEN (36+1)

/* library versioning */

/**
 * LIBVIR_VERSION_NUMBER:
 *
 * Macro providing the version of the library as
 * version * 1,000,000 + minor * 1000 + micro
 */

#define LIBVIR_VERSION_NUMBER 4006

int                     virGetVersion           (unsigned long *libVer,
                                                 const char *type,
                                                 unsigned long *typeVer);

/*
 * Connection and disconnections to the Hypervisor
 */
int                     virInitialize           (void);

virConnectPtr           virConnectOpen          (const char *name);
virConnectPtr           virConnectOpenReadOnly  (const char *name);
virConnectPtr           virConnectOpenAuth      (const char *name,
                                                 virConnectAuthPtr auth,
                                                 int flags);
int                     virConnectClose         (virConnectPtr conn);
const char *            virConnectGetType       (virConnectPtr conn);
int                     virConnectGetVersion    (virConnectPtr conn,
                                                 unsigned long *hvVer);
char *                  virConnectGetHostname   (virConnectPtr conn);
char *                  virConnectGetURI        (virConnectPtr conn);


/*
 * Capabilities of the connection / driver.
 */

int                     virConnectGetMaxVcpus   (virConnectPtr conn,
                                                 const char *type);
int                     virNodeGetInfo          (virConnectPtr conn,
                                                 virNodeInfoPtr info);
char *                  virConnectGetCapabilities (virConnectPtr conn);

unsigned long long      virNodeGetFreeMemory    (virConnectPtr conn);

/*
 * Gather list of running domains
 */
int                     virConnectListDomains   (virConnectPtr conn,
                                                 int *ids,
                                                 int maxids);

/*
 * Number of domains
 */
int                     virConnectNumOfDomains  (virConnectPtr conn);


/*
 * Get connection from domain.
 */
virConnectPtr           virDomainGetConnect     (virDomainPtr domain);

/*
 * Domain creation and destruction
 */
virDomainPtr            virDomainCreateXML      (virConnectPtr conn,
                                                 const char *xmlDesc,
                                                 unsigned int flags);
virDomainPtr            virDomainLookupByName   (virConnectPtr conn,
                                                 const char *name);
virDomainPtr            virDomainLookupByID     (virConnectPtr conn,
                                                 int id);
virDomainPtr            virDomainLookupByUUID   (virConnectPtr conn,
                                                 const unsigned char *uuid);
virDomainPtr            virDomainLookupByUUIDString     (virConnectPtr conn,
                                                        const char *uuid);

int                     virDomainShutdown       (virDomainPtr domain);
int                     virDomainReboot         (virDomainPtr domain,
                                                 unsigned int flags);
int                     virDomainDestroy        (virDomainPtr domain);
int                     virDomainFree           (virDomainPtr domain);

/*
 * Domain suspend/resume
 */
int                     virDomainSuspend        (virDomainPtr domain);
int                     virDomainResume         (virDomainPtr domain);

/*
 * Domain save/restore
 */
int                     virDomainSave           (virDomainPtr domain,
                                                 const char *to);
int                     virDomainRestore        (virConnectPtr conn,
                                                 const char *from);

/*
 * Domain core dump
 */
int                     virDomainCoreDump       (virDomainPtr domain,
                                                 const char *to,
                                                 int flags);

/*
 * Domain runtime information
 */
int                     virDomainGetInfo        (virDomainPtr domain,
                                                 virDomainInfoPtr info);

/*
 * Return scheduler type in effect 'sedf', 'credit', 'linux'
 */
char *                  virDomainGetSchedulerType(virDomainPtr domain,
                                                 int *nparams);

/*
 * Dynamic control of domains
 */
const char *            virDomainGetName        (virDomainPtr domain);
unsigned int            virDomainGetID          (virDomainPtr domain);
int                     virDomainGetUUID        (virDomainPtr domain,
                                                 unsigned char *uuid);
int                     virDomainGetUUIDString  (virDomainPtr domain,
                                                char *buf);
char *                  virDomainGetOSType      (virDomainPtr domain);
unsigned long           virDomainGetMaxMemory   (virDomainPtr domain);
int                     virDomainSetMaxMemory   (virDomainPtr domain,
                                                 unsigned long memory);
int                     virDomainSetMemory      (virDomainPtr domain,
                                                 unsigned long memory);
int                     virDomainGetMaxVcpus    (virDomainPtr domain);

/*
 * XML domain description
 */
/**
 * virDomainXMLFlags:
 *
 * Flags available for virDomainGetXMLDesc
 */

typedef enum {
    VIR_DOMAIN_XML_SECURE = 1, /* dump security sensitive information too */
    VIR_DOMAIN_XML_INACTIVE = 2/* dump inactive domain information */
} virDomainXMLFlags;

char *                  virDomainGetXMLDesc     (virDomainPtr domain,
                                                 int flags);

int                     virDomainBlockStats     (virDomainPtr dom,
                                                 const char *path,
                                                 virDomainBlockStatsPtr stats,
                                                 size_t size);
int                     virDomainInterfaceStats (virDomainPtr dom,
                                                 const char *path,
                                                 virDomainInterfaceStatsPtr stats,
                                                 size_t size);
int                     virDomainBlockPeek (virDomainPtr dom,
                                            const char *path,
                                            unsigned long long offset,
                                            size_t size,
                                            void *buffer,
                                            unsigned int flags);

/* Memory peeking flags. */
typedef enum {
  VIR_MEMORY_VIRTUAL              = 1, /* addresses are virtual addresses */
} virDomainMemoryFlags;

int                     virDomainMemoryPeek (virDomainPtr dom,
                                             unsigned long long start,
                                             size_t size,
                                             void *buffer,
                                             unsigned int flags);

/*
 * defined but not running domains
 */
virDomainPtr            virDomainDefineXML      (virConnectPtr conn,
                                                 const char *xml);
int                     virDomainUndefine       (virDomainPtr domain);
int                     virConnectNumOfDefinedDomains  (virConnectPtr conn);
int                     virConnectListDefinedDomains (virConnectPtr conn,
                                                 char **const names,
                                                 int maxnames);
int                     virDomainCreate         (virDomainPtr domain);

int                     virDomainGetAutostart   (virDomainPtr domain,
                                                 int *autostart);
int                     virDomainSetAutostart   (virDomainPtr domain,
                                                 int autostart);

/**
 * virVcpuInfo: structure for information about a virtual CPU in a domain.
 */

typedef enum {
    VIR_VCPU_OFFLINE    = 0,    /* the virtual CPU is offline */
    VIR_VCPU_RUNNING    = 1,    /* the virtual CPU is running */
    VIR_VCPU_BLOCKED    = 2,    /* the virtual CPU is blocked on resource */
} virVcpuState;

typedef struct _virVcpuInfo virVcpuInfo;
struct _virVcpuInfo {
    unsigned int number;        /* virtual CPU number */
    int state;                  /* value from virVcpuState */
    unsigned long long cpuTime; /* CPU time used, in nanoseconds */
    int cpu;                    /* real CPU number, or -1 if offline */
};
typedef virVcpuInfo *virVcpuInfoPtr;

int                     virDomainSetVcpus       (virDomainPtr domain,
                                                 unsigned int nvcpus);

int                     virDomainPinVcpu        (virDomainPtr domain,
                                                 unsigned int vcpu,
                                                 unsigned char *cpumap,
                                                 int maplen);

/**
 * VIR_USE_CPU:
 * @cpumap: pointer to a bit map of real CPUs (in 8-bit bytes) (IN/OUT)
 * @cpu: the physical CPU number
 *
 * This macro is to be used in conjunction with virDomainPinVcpu() API.
 * USE_CPU macro set the bit (CPU usable) of the related cpu in cpumap.
 */

#define VIR_USE_CPU(cpumap,cpu) (cpumap[(cpu)/8] |= (1<<((cpu)%8)))

/**
 * VIR_UNUSE_CPU:
 * @cpumap: pointer to a bit map of real CPUs (in 8-bit bytes) (IN/OUT)
 * @cpu: the physical CPU number
 *
 * This macro is to be used in conjunction with virDomainPinVcpu() API.
 * USE_CPU macro reset the bit (CPU not usable) of the related cpu in cpumap.
 */

#define VIR_UNUSE_CPU(cpumap,cpu)       (cpumap[(cpu)/8] &= ~(1<<((cpu)%8)))

/**
 * VIR_CPU_MAPLEN:
 * @cpu: number of physical CPUs
 *
 * This macro is to be used in conjunction with virDomainPinVcpu() API.
 * It returns the length (in bytes) required to store the complete
 * CPU map between a single virtual & all physical CPUs of a domain.
 */

#define VIR_CPU_MAPLEN(cpu)      (((cpu)+7)/8)


int                     virDomainGetVcpus       (virDomainPtr domain,
                                                 virVcpuInfoPtr info,
                                                 int maxinfo,
                                                 unsigned char *cpumaps,
                                                 int maplen);

/**
 * VIR_CPU_USABLE:
 * @cpumaps: pointer to an array of cpumap (in 8-bit bytes) (IN)
 * @maplen: the length (in bytes) of one cpumap
 * @vcpu: the virtual CPU number
 * @cpu: the physical CPU number
 *
 * This macro is to be used in conjunction with virDomainGetVcpus() API.
 * VIR_CPU_USABLE macro returns a non zero value (true) if the cpu
 * is usable by the vcpu, and 0 otherwise.
 */

#define VIR_CPU_USABLE(cpumaps,maplen,vcpu,cpu) \
        (cpumaps[((vcpu)*(maplen))+((cpu)/8)] & (1<<((cpu)%8)))

/**
 * VIR_COPY_CPUMAP:
 * @cpumaps: pointer to an array of cpumap (in 8-bit bytes) (IN)
 * @maplen: the length (in bytes) of one cpumap
 * @vcpu: the virtual CPU number
 * @cpumap: pointer to a cpumap (in 8-bit bytes) (OUT)
 *      This cpumap must be previously allocated by the caller
 *      (ie: malloc(maplen))
 *
 * This macro is to be used in conjunction with virDomainGetVcpus() and
 * virDomainPinVcpu() APIs. VIR_COPY_CPUMAP macro extract the cpumap of
 * the specified vcpu from cpumaps array and copy it into cpumap to be used
 * later by virDomainPinVcpu() API.
 */
#define VIR_COPY_CPUMAP(cpumaps,maplen,vcpu,cpumap) \
        memcpy(cpumap, &(cpumaps[(vcpu)*(maplen)]), (maplen))


/**
 * VIR_GET_CPUMAP:
 * @cpumaps: pointer to an array of cpumap (in 8-bit bytes) (IN)
 * @maplen: the length (in bytes) of one cpumap
 * @vcpu: the virtual CPU number
 *
 * This macro is to be used in conjunction with virDomainGetVcpus() and
 * virDomainPinVcpu() APIs. VIR_GET_CPUMAP macro returns a pointer to the
 * cpumap of the specified vcpu from cpumaps array.
 */
#define VIR_GET_CPUMAP(cpumaps,maplen,vcpu)     &(cpumaps[(vcpu)*(maplen)])

int virDomainAttachDevice(virDomainPtr domain, const char *xml);
int virDomainDetachDevice(virDomainPtr domain, const char *xml);

/*
 * NUMA support
 */

int                      virNodeGetCellsFreeMemory(virConnectPtr conn,
                                                   unsigned long long *freeMems,
                                                   int startCell,
                                                   int maxCells);

/*
 * Virtual Networks API
 */

/**
 * virNetwork:
 *
 * a virNetwork is a private structure representing a virtual network.
 */
typedef struct _virNetwork virNetwork;

/**
 * virNetworkPtr:
 *
 * a virNetworkPtr is pointer to a virNetwork private structure, this is the
 * type used to reference a virtual network in the API.
 */
typedef virNetwork *virNetworkPtr;

/*
 * Get connection from network.
 */
virConnectPtr           virNetworkGetConnect    (virNetworkPtr network);

/*
 * List active networks
 */
int                     virConnectNumOfNetworks (virConnectPtr conn);
int                     virConnectListNetworks  (virConnectPtr conn,
                                                 char **const names,
                                                 int maxnames);

/*
 * List inactive networks
 */
int                     virConnectNumOfDefinedNetworks  (virConnectPtr conn);
int                     virConnectListDefinedNetworks   (virConnectPtr conn,
                                                         char **const names,
                                                         int maxnames);

/*
 * Lookup network by name or uuid
 */
virNetworkPtr           virNetworkLookupByName          (virConnectPtr conn,
                                                         const char *name);
virNetworkPtr           virNetworkLookupByUUID          (virConnectPtr conn,
                                                         const unsigned char *uuid);
virNetworkPtr           virNetworkLookupByUUIDString    (virConnectPtr conn,
                                                         const char *uuid);

/*
 * Create active transient network
 */
virNetworkPtr           virNetworkCreateXML     (virConnectPtr conn,
                                                 const char *xmlDesc);

/*
 * Define inactive persistent network
 */
virNetworkPtr           virNetworkDefineXML     (virConnectPtr conn,
                                                 const char *xmlDesc);

/*
 * Delete persistent network
 */
int                     virNetworkUndefine      (virNetworkPtr network);

/*
 * Activate persistent network
 */
int                     virNetworkCreate        (virNetworkPtr network);

/*
 * Network destroy/free
 */
int                     virNetworkDestroy       (virNetworkPtr network);
int                     virNetworkFree          (virNetworkPtr network);

/*
 * Network information
 */
const char*             virNetworkGetName       (virNetworkPtr network);
int                     virNetworkGetUUID       (virNetworkPtr network,
                                                 unsigned char *uuid);
int                     virNetworkGetUUIDString (virNetworkPtr network,
                                                 char *buf);
char *                  virNetworkGetXMLDesc    (virNetworkPtr network,
                                                 int flags);
char *                  virNetworkGetBridgeName (virNetworkPtr network);

int                     virNetworkGetAutostart  (virNetworkPtr network,
                                                 int *autostart);
int                     virNetworkSetAutostart  (virNetworkPtr network,
                                                 int autostart);


/**
 * virStoragePool:
 *
 * a virStoragePool is a private structure representing a storage pool
 */
typedef struct _virStoragePool virStoragePool;

/**
 * virStoragePoolPtr:
 *
 * a virStoragePoolPtr is pointer to a virStoragePool private structure, this is the
 * type used to reference a storage pool in the API.
 */
typedef virStoragePool *virStoragePoolPtr;


typedef enum {
  VIR_STORAGE_POOL_INACTIVE = 0, /* Not running */
  VIR_STORAGE_POOL_BUILDING = 1, /* Initializing pool, not available */
  VIR_STORAGE_POOL_RUNNING = 2,  /* Running normally */
  VIR_STORAGE_POOL_DEGRADED = 3, /* Running degraded */
} virStoragePoolState;


typedef enum {
  VIR_STORAGE_POOL_BUILD_NEW  = 0,   /* Regular build from scratch */
  VIR_STORAGE_POOL_BUILD_REPAIR = 1, /* Repair / reinitialize */
  VIR_STORAGE_POOL_BUILD_RESIZE = 2  /* Extend existing pool */
} virStoragePoolBuildFlags;

typedef enum {
  VIR_STORAGE_POOL_DELETE_NORMAL = 0, /* Delete metadata only    (fast) */
  VIR_STORAGE_POOL_DELETE_ZEROED = 1,  /* Clear all data to zeros (slow) */
} virStoragePoolDeleteFlags;

typedef struct _virStoragePoolInfo virStoragePoolInfo;

struct _virStoragePoolInfo {
  int state;                     /* virStoragePoolState flags */
  unsigned long long capacity;   /* Logical size bytes */
  unsigned long long allocation; /* Current allocation bytes */
  unsigned long long available;  /* Remaining free space bytes */
};

typedef virStoragePoolInfo *virStoragePoolInfoPtr;


/**
 * virStorageVol:
 *
 * a virStorageVol is a private structure representing a storage volume
 */
typedef struct _virStorageVol virStorageVol;

/**
 * virStorageVolPtr:
 *
 * a virStorageVolPtr is pointer to a virStorageVol private structure, this is the
 * type used to reference a storage volume in the API.
 */
typedef virStorageVol *virStorageVolPtr;


typedef enum {
  VIR_STORAGE_VOL_FILE = 0,     /* Regular file based volumes */
  VIR_STORAGE_VOL_BLOCK = 1,    /* Block based volumes */
} virStorageVolType;

typedef enum {
  VIR_STORAGE_VOL_DELETE_NORMAL = 0, /* Delete metadata only    (fast) */
  VIR_STORAGE_VOL_DELETE_ZEROED = 1,  /* Clear all data to zeros (slow) */
} virStorageVolDeleteFlags;

typedef struct _virStorageVolInfo virStorageVolInfo;

struct _virStorageVolInfo {
  int type;                      /* virStorageVolType flags */
  unsigned long long capacity;   /* Logical size bytes */
  unsigned long long allocation; /* Current allocation bytes */
};

typedef virStorageVolInfo *virStorageVolInfoPtr;

/*
 * Get connection from pool.
 */
virConnectPtr           virStoragePoolGetConnect        (virStoragePoolPtr pool);

/*
 * List active storage pools
 */
int                     virConnectNumOfStoragePools     (virConnectPtr conn);
int                     virConnectListStoragePools      (virConnectPtr conn,
                                                         char **const names,
                                                         int maxnames);

/*
 * List inactive storage pools
 */
int                     virConnectNumOfDefinedStoragePools(virConnectPtr conn);
int                     virConnectListDefinedStoragePools(virConnectPtr conn,
                                                          char **const names,
                                                          int maxnames);

/*
 * Query a host for storage pools of a particular type
 */
char *                  virConnectFindStoragePoolSources(virConnectPtr conn,
                                                         const char *type,
                                                         const char *srcSpec,
                                                         unsigned int flags);

/*
 * Lookup pool by name or uuid
 */
virStoragePoolPtr       virStoragePoolLookupByName      (virConnectPtr conn,
                                                         const char *name);
virStoragePoolPtr       virStoragePoolLookupByUUID      (virConnectPtr conn,
                                                         const unsigned char *uuid);
virStoragePoolPtr       virStoragePoolLookupByUUIDString(virConnectPtr conn,
                                                         const char *uuid);
virStoragePoolPtr       virStoragePoolLookupByVolume    (virStorageVolPtr vol);

/*
 * Creating/destroying pools
 */
virStoragePoolPtr       virStoragePoolCreateXML         (virConnectPtr conn,
                                                         const char *xmlDesc,
                                                         unsigned int flags);
virStoragePoolPtr       virStoragePoolDefineXML         (virConnectPtr conn,
                                                         const char *xmlDesc,
                                                         unsigned int flags);
int                     virStoragePoolBuild             (virStoragePoolPtr pool,
                                                         unsigned int flags);
int                     virStoragePoolUndefine          (virStoragePoolPtr pool);
int                     virStoragePoolCreate            (virStoragePoolPtr pool,
                                                         unsigned int flags);
int                     virStoragePoolDestroy           (virStoragePoolPtr pool);
int                     virStoragePoolDelete            (virStoragePoolPtr pool,
                                                         unsigned int flags);
int                     virStoragePoolFree              (virStoragePoolPtr pool);
int                     virStoragePoolRefresh           (virStoragePoolPtr pool,
                                                         unsigned int flags);

/*
 * StoragePool information
 */
const char*             virStoragePoolGetName           (virStoragePoolPtr pool);
int                     virStoragePoolGetUUID           (virStoragePoolPtr pool,
                                                         unsigned char *uuid);
int                     virStoragePoolGetUUIDString     (virStoragePoolPtr pool,
                                                         char *buf);

int                     virStoragePoolGetInfo           (virStoragePoolPtr vol,
                                                         virStoragePoolInfoPtr info);

char *                  virStoragePoolGetXMLDesc        (virStoragePoolPtr pool,
                                                         unsigned int flags);

int                     virStoragePoolGetAutostart      (virStoragePoolPtr pool,
                                                         int *autostart);
int                     virStoragePoolSetAutostart      (virStoragePoolPtr pool,
                                                         int autostart);

/*
 * List/lookup storage volumes within a pool
 */
int                     virStoragePoolNumOfVolumes      (virStoragePoolPtr pool);
int                     virStoragePoolListVolumes       (virStoragePoolPtr pool,
                                                         char **const names,
                                                         int maxnames);

virConnectPtr           virStorageVolGetConnect         (virStorageVolPtr vol);

/*
 * Lookup volumes based on various attributes
 */
virStorageVolPtr        virStorageVolLookupByName       (virStoragePoolPtr pool,
                                                         const char *name);
virStorageVolPtr        virStorageVolLookupByKey        (virConnectPtr conn,
                                                         const char *key);
virStorageVolPtr        virStorageVolLookupByPath       (virConnectPtr conn,
                                                         const char *path);


const char*             virStorageVolGetName            (virStorageVolPtr vol);
const char*             virStorageVolGetKey             (virStorageVolPtr vol);

virStorageVolPtr        virStorageVolCreateXML          (virStoragePoolPtr pool,
                                                         const char *xmldesc,
                                                         unsigned int flags);
int                     virStorageVolDelete             (virStorageVolPtr vol,
                                                         unsigned int flags);
int                     virStorageVolFree               (virStorageVolPtr vol);

int                     virStorageVolGetInfo            (virStorageVolPtr vol,
                                                         virStorageVolInfoPtr info);
char *                  virStorageVolGetXMLDesc         (virStorageVolPtr pool,
                                                         unsigned int flags);

char *                  virStorageVolGetPath            (virStorageVolPtr vol);

/*
 * Deprecated calls
 */
virDomainPtr            virDomainCreateLinux    (virConnectPtr conn,
                                                 const char *xmlDesc,
                                                 unsigned int flags);
#ifdef __cplusplus
}
#endif

#endif /* __VIR_VIRLIB_H__ */
