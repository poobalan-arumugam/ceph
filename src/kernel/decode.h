#ifndef __CEPH_DECODE_H
#define __CEPH_DECODE_H

/*
 * in all cases, 
 *   void **p     pointer to position pointer
 *   void *end    pointer to end of buffer (last byte + 1)
 */

/*
 * bounds check input.
 */
#define ceph_decode_need(p, end, n, bad)		\
	do {						\
		if (unlikely(*(p) + (n) > (end))) 	\
			goto bad;			\
	} while (0)

#define ceph_decode_64(p, v)				\
	do {						\
		v = le64_to_cpu(*(__u64*)*(p));		\
		*(p) += sizeof(__u64);			\
	} while (0)
#define ceph_decode_32(p, v)				\
	do {						\
		v = le32_to_cpu(*(__u32*)*(p));		\
		*(p) += sizeof(__u32);			\
	} while (0)
#define ceph_decode_16(p, v)				\
	do {						\
		v = le16_to_cpu(*(__u16*)*(p));		\
		*(p) += sizeof(__u16);			\
	} while (0)
#define ceph_decode_8(p, v)				\
	do {						\
		v = *(__u8*)*(p);			\
		(*p)++;					\
	} while (0)

#define ceph_decode_copy(p, pv, n)			\
	do {						\
		memcpy(pv, *(p), n);			\
		*(p) += n;				\
	} while (0)

#define ceph_decode_64_safe(p, end, v, bad)			\
	do {							\
		ceph_decode_need(p, end, sizeof(__u64), bad);	\
		ceph_decode_64(p, v);				\
	} while (0)
#define ceph_decode_32_safe(p, end, v, bad)			\
	do {							\
		ceph_decode_need(p, end, sizeof(__u32), bad);	\
		ceph_decode_32(p, v);				\
	} while (0)
#define ceph_decode_16_safe(p, end, v, bad)			\
	do {							\
		ceph_decode_need(p, end, sizeof(__u16), bad);	\
		ceph_decode_16(p, v);				\
	} while (0)

#define ceph_decode_copy_safe(p, end, pv, n, bad)		\
	do {							\
		ceph_decode_need(p, end, n, bad);		\
		ceph_decode_copy(p, pv, n);			\
	} while (0)

/*
 * struct ceph_timespec <-> struct timespec
 */ 
#define ceph_decode_timespec(ts, tv)					\
	do {								\
		(ts)->tv_sec = le32_to_cpu((tv)->tv_sec);		\
		(ts)->tv_nsec = le32_to_cpu((tv)->tv_nsec);		\
	} while (0)
#define ceph_encode_timespec(tv, ts)				\
	do {							\
		(tv)->tv_sec = cpu_to_le32((ts)->tv_sec);	\
		(tv)->tv_nsec = cpu_to_le32((ts)->tv_nsec);	\
	} while (0)


/*
 * encoders
 */ 

#define ceph_encode_64(p, v)			  \
	do {					  \
		*(__u64*)*(p) = cpu_to_le64((v)); \
		*(p) += sizeof(__u64);		  \
	} while (0)
#define ceph_encode_32(p, v)			  \
	do {					  \
		*(__u32*)*(p) = cpu_to_le32((v)); \
		*(p) += sizeof(__u32);		  \
	} while (0)
#define ceph_encode_16(p, v)			  \
	do {					  \
		*(__u16*)*(p) = cpu_to_le16((v)); \
		*(p) += sizeof(__u16);		  \
	} while (0)
#define ceph_encode_8(p, v)			  \
	do {					  \
		*(__u8*)*(p) = v;		  \
		(*(p))++;			  \
	} while (0)

/* 
 * filepath, string encoders 
 */

static __inline__ void ceph_encode_filepath(void **p, void *end, 
					    ceph_ino_t ino, const char *path)
{
	__u32 len = path ? strlen(path):0;
	BUG_ON(*p + sizeof(ino) + sizeof(len) + len > end);
	ceph_encode_64(p, ino);
	ceph_encode_32(p, len);
	if (len) 
		memcpy(*p, path, len);
	*p += len;
}

static __inline__ void ceph_encode_string(void **p, void *end, 
					  const char *s, __u32 len)
{
	BUG_ON(*p + sizeof(len) + len > end);
	ceph_encode_32(p, len);
	if (len)
		memcpy(*p, s, len);
	*p += len;
}


#endif
