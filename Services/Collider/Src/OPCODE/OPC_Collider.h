///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains base collider class.
 *	\file		OPC_Collider.h
 *	\author		Pierre Terdiman
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_COLLIDER_H__
#define __OPC_COLLIDER_H__

	class OPCODE_API Collider
	{
		public:
		// Constructor / Destructor
											Collider();
		virtual								~Collider();

		// Collision report

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the last collision status after a collision query.
		 *	\return		true if a collision occured
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	bool				GetContactStatus()				const	{ return mContact;					}

		// Settings

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Reports all contacts (false) or first contact only (true)
		 *	\param		flag		[in] true for first contact, false for all contacts
		 *	\see		SetTemporalCoherence(bool flag)
		 *	\see		ValidateSettings()
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	void				SetFirstContact(bool flag)				{ mFirstContact			= flag;		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Enable/disable temporal coherence.
		 *	\param		flag		[in] true to enable temporal coherence, false to discard it
		 *	\see		SetFirstContact(bool flag)
		 *	\see		ValidateSettings()
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		__forceinline	void				SetTemporalCoherence(bool flag)			{ mTemporalCoherence	= flag;		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Validates current settings. You should call this method after all the settings / callbacks have been defined for a collider.
		 *	\return		null if everything is ok, else a string describing the problem
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual			const char*			ValidateSettings()	= 0
		// 379: gcc doesn't allow a function body on a pure virtual
#if !defined(__GNUC__)
						{
							return "Collider::ValidateSettings: pure virtual function called!";
						}
#else
;
#endif

		protected:
		// Settings
						bool				mFirstContact;			//!< Report all contacts (false) or only first one (true)
						bool				mTemporalCoherence;		//!< Use temporal coherence or not

		// Collision result
						bool				mContact;				//!< Final contact status after a collision query
	};

#endif // __OPC_COLLIDER_H__
